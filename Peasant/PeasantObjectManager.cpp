////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantObjectManager.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PeasantObjectManager.h"
#include "PeasantInstance.h"
#include <cassert>

// Using namespace Peasant
PeasantUsingDevelopmentNamespace(Peasant)

PeasantObjectManager::PeasantObjectManager(PeasantStorage& _storageReference, uint32_t _workerThreads, ThreadIndexRetrieveMethod _threadIndexMethod, ObjectLoadMethod _objectLoadMethod) : 
	m_StorageReference(_storageReference),
	m_ObjectLoader(_objectLoadMethod)
{
	// If we have at last one worker thread
	if (_workerThreads > 1)
	{
		// Set the initial data
		m_ObjectRequests.AllowThreadedAccess(_workerThreads, _threadIndexMethod);
		m_InstanceReleases.AllowThreadedAccess(_workerThreads, _threadIndexMethod);
	}

	// Set the initial data
	m_InUpdatePhase = false;
}

PeasantObjectManager::~PeasantObjectManager()
{
}

bool PeasantObjectManager::RequestObject(PeasantInstance* _instance, PeasantHash _hash, PeasantObjectFactory* _factoryPtr, bool _allowAsynchronousConstruct)
{
	// Asserts
	assert(!m_InUpdatePhase || (m_InUpdatePhase && std::this_thread::get_id() == m_UpdateThreadID));

	// Register the hash and this reference for this instance
	_instance->RegisterInfo(_hash, this);

	// Check if we already have an object with this hash, if the ibject was loaded and if we can construct this instance asynchronous
	PeasantObject* object = m_StorageReference.FindObject(_hash);
	if (object != nullptr && object->WasLoaded() && object->WasSynchronized() && _allowAsynchronousConstruct)
	{
		// Make the instance reference it
		object->MakeInstanceReference(_instance);

		// Construct this instance
		_instance->BeginConstruction();
	}
	else
	{
		// Create the new request
		ObjectRequest request = { _instance, _hash, _factoryPtr, false };

		// Push the new request
		m_ObjectRequests.Insert(request);
	}

	return true;
}

bool PeasantObjectManager::RequestPersistentObject(PeasantInstance* _instance, PeasantHash _hash, PeasantObjectFactory* _factoryPtr, bool _allowAsynchronousConstruct)
{
	// Asserts
	assert(!m_InUpdatePhase || (m_InUpdatePhase && std::this_thread::get_id() == m_UpdateThreadID));

	// Register the hash and this reference for this instance
	_instance->RegisterInfo(_hash, this);

	// Check if we already have an object with this hash, if the ibject was loaded and if we can construct this instance asynchronous
	PeasantObject* object = m_StorageReference.FindObject(_hash);
	if (object != nullptr && object->WasLoaded() && object->WasSynchronized() && _allowAsynchronousConstruct)
	{
		// Check if the object is permanent
		assert(!object->IsPersistent() && "Trying to request a permanent object but it was already loaded and is not permanent!");

		// Make the instance reference it
		object->MakeInstanceReference(_instance);

		// Construct this instance
		_instance->BeginConstruction();
	}
	else
	{
		// Create the new request
		ObjectRequest request = { _instance, _hash, _factoryPtr, true };

		// Push the new request
		m_ObjectRequests.Insert(request);
	}

	return true;
}

void PeasantObjectManager::ReleaseObject(PeasantInstance* _instance, PeasantObjectFactory* _factoryPtr, bool _allowAsynchronousDeletion)
{
	// Asserts
	assert(!m_InUpdatePhase || (m_InUpdatePhase && std::this_thread::get_id() == m_UpdateThreadID));

	// Create the new release request
	ObjectRelease release = { _instance->GetObjectPtr(), _factoryPtr, !_allowAsynchronousDeletion };

	// Push the new release request
	m_InstanceReleases.Insert(release);
}

void PeasantObjectManager::ReleaseObject(PeasantObject* _object, PeasantObjectFactory* _factoryPtr, bool _allowAsynchronousDeletion)
{
	// Asserts
	assert(!m_InUpdatePhase || (m_InUpdatePhase && std::this_thread::get_id() == m_UpdateThreadID));

	// Create the new temporary release request
	TemporaryObjectRelease release = { _object, _factoryPtr, !_allowAsynchronousDeletion };

	// Push the new release request
	m_TemporaryInstanceReleases.Insert(release);
}

void PeasantObjectManager::Update()
{
	// Prevent multiple threads from running this code (only one thread allowed, take care!)
	std::lock_guard<std::mutex> guard(m_Mutex);

	// Begin the update phase
	m_InUpdatePhase = true;
	m_UpdateThreadID = std::this_thread::get_id();

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

			// Set the object factory reference
			object->SetFactoryReference(_requestData.factoryPtr);

			// Insert this file into the load queue
			m_ObjectLoader.LoadObject(object, _requestData.hash, _requestData.isPermanent);

			// Insert the new object into the storage
			m_StorageReference.InsertObject(object, _requestData.hash);
		}

		// Make the instance reference it
		object->MakeInstanceReference(_requestData.instance);

		// Insert the reference into the construct queue
		m_ConstructQueue.push_back(_requestData.instance);

	}, true);

	// For each release, run the process method
	m_InstanceReleases.ProcessAll([&](ObjectRelease _releaseRequest)
	{
		// Release this instance
		_releaseRequest.object->ReleaseInstance();

		// Check if the object should be deleted
		if (!_releaseRequest.object->IsReferenced() && !_releaseRequest.object->IsPersistent())
		{
			// Remove this object from the storage
			m_StorageReference.RemoveObject(_releaseRequest.object);

			// Add this object into the deletion queue
			m_ObjectDeleter.DeleteObject(_releaseRequest.object, _releaseRequest.factoryPtr, _releaseRequest.deleteSync);
		}

	}, true);

	// For each temporary release, run the process method
	m_TemporaryInstanceReleases.ProcessAll([&](TemporaryObjectRelease _releaseRequest)
	{
		// Release this instance
		_releaseRequest.object->ReleaseInstance();

		// Check if the object should be deleted
		if (!_releaseRequest.object->IsReferenced() && !_releaseRequest.object->IsPersistent())
		{
			// Remove this object from the storage
			m_StorageReference.RemoveObject(_releaseRequest.object);

			// Add this object into the deletion queue
			m_ObjectDeleter.DeleteObject(_releaseRequest.object, _releaseRequest.factoryPtr, _releaseRequest.deleteSync);
		}

	}, true);

	// Process all construct objects
	for (unsigned int i = 0; i < m_ConstructQueue.size(); i++)
	{
		// Get the instance
		PeasantInstance* instance = m_ConstructQueue[i];

		// Check if the internal object was loaded (and synchronized) and this instance is ready to be constructed
		if (instance->WasLoaded() && instance->WasSynchronized())
		{
			// Construct this instance
			instance->BeginConstruction();

			// Remove it from the construct queue
			m_ConstructQueue.erase(m_ConstructQueue.begin() + i);

			// Return 1 from the current index
			i--;
		}
	}

	// Call the update method for the object deleter and loader
	m_ObjectDeleter.Update();
	m_ObjectLoader.Update();

	// End the update phase
	m_InUpdatePhase = false;
}