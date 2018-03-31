////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantObjectManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PeasantObjectManager.h"
#include <cassert>

// Using namespace Peasant
PeasantUsingDevelopmentNamespace(Peasant)

PeasantObjectManager::PeasantObjectManager(PeasantStorage& _storageReference, uint32_t _workerThreads, ThreadIndexRetrieveMethod _threadIndexMethod, ObjectLoadMethod _objectLoadMethod, ObjectDeleteMethod _objectDeletionMethod) : 
	m_StorageReference(_storageReference),
	m_ObjectLoader(_objectLoadMethod),
	m_ObjectDeleter(_objectDeletionMethod)
{
	// If we have at last one worker thread
	if (_workerThreads > 1)
	{
		// Set the initial data
		m_ObjectRequests.AllowThreadedAccess(_workerThreads, _threadIndexMethod);
		m_InstanceReleases.AllowThreadedAccess(_workerThreads, _threadIndexMethod);
	}
}

PeasantObjectManager::~PeasantObjectManager()
{
}

bool PeasantObjectManager::RequestObject(PeasantInstance* _instance, PeasantHash _hash, PeasantObjectFactory* _factoryPtr)
{
	// Create the new request
	ObjectRequest request = { _instance, _hash, _factoryPtr };

	// Push the new request
	m_ObjectRequests.Insert(request);

	return true;
}

void PeasantObjectManager::ReleaseObject(PeasantInstance* _instance)
{
	m_InstanceReleases.Insert(_instance);
}

void PeasantObjectManager::Update()
{
	// Prevent multiple threads from running this code (only one thread allowed, take care!)
	// std::lock_guard<std::mutex> guard(m_Mutex);

	// Process all construct objects
	for (unsigned int i = 0; i < m_ConstructQueue.size(); i++)
	{
		// Get the instance
		PeasantInstance* instance = m_ConstructQueue[i];

		// Check if the internal object was loaded and this instance is ready to be constructed
		if (instance->WasLoaded())
		{
			// Construct this instance
			instance->BeginConstruction();

			// Remove it from the construct queue
			m_ConstructQueue.erase(m_ConstructQueue.begin() + 1);

			// Return 1 from the current index
			i--;
		}
	}

	// For each request, run the process method
	m_ObjectRequests.ProcessAll([&](ObjectRequest& _requestData)
	{
		// Check if we already have an object with this hash
		PeasantObject* object = m_StorageReference.FindObject(_requestData.hash);
		if (object == nullptr)
		{
			// Create a new object (use the factory)
			object = _requestData.factoryPtr->RequestObject();
			assert(object != nullptr);

			// Set the object hash
			object->SetHash(_requestData.hash);

			// Add the instance to the object alert callback
			// ... TODO

			// Insert this file into the load queue
			m_ObjectLoader.LoadObject(object, _requestData.hash);
		}

		// Mae the instance reference it
		object->MakeInstanceReference(_requestData.instance);

		// Insert the reference into the construct queue
		m_ConstructQueue.push_back(_requestData.instance);

	}, true);

	// For each release, run the process method
	m_InstanceReleases.ProcessAll([&](PeasantInstance* _instance)
	{
		// Get the internal object ptr
		PeasantObject* objectPtr = _instance->GetObjectPtr();

		// Release this instance
		objectPtr->ReleaseInstance(_instance);

		// Check if the object should be deleted
		if (!objectPtr->IsReferenced())
		{
			// Remove this object from the storage
			m_StorageReference.RemoveObject(objectPtr);

			// Add this object into the deletion queue
			m_ObjectDeleter.DeleteObject(objectPtr);
		}

	}, true);
}