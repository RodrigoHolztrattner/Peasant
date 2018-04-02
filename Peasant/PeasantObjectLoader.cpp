////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantObjectLoader.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PeasantObjectLoader.h"
#include <cassert>
#include <chrono>

// Using namespace Peasant
PeasantUsingDevelopmentNamespace(Peasant)

PeasantObjectLoader::PeasantObjectLoader(ObjectLoadMethod _objectLoadMethod)
{
	// Set the initial data
	m_LoadMethod = _objectLoadMethod;

	// Create the auxiliar thread
	m_AuxiliarThread = std::thread(&PeasantObjectLoader::LoadObjectAuxiliar, this);
}

PeasantObjectLoader::~PeasantObjectLoader()
{
}

bool PeasantObjectLoader::LoadObject(PeasantObject* _object, PeasantHash _hash)
{
	// Set the data
	LoadData loadData = { _object, _hash };

	return m_Queue.enqueue(loadData);
}

void PeasantObjectLoader::Update()
{
	// For each object inside our synchronization queue
	PeasantObject* object = nullptr;
	while (m_SynchronizationQueue.try_dequeue(object))
	{
		// Call the BeginSynchronization() method
		object->BeginSynchronization();
	}
}

void PeasantObjectLoader::LoadObjectAuxiliar()
{
	// Do forever
	while (true)
	{
		// Try to get an object from the queue
		LoadData loadData;
		if (!m_Queue.try_dequeue(loadData))
		{
			// Sleep because there is no object to dequeue
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			continue;
		}

		// Load this object
		bool result = m_LoadMethod(loadData.object->GetDataPtr(), loadData.object->GetDataSizeRef(), loadData.hash);

		assert(result == true);

		// Call the BeginLoad() method for this object
		result = loadData.object->BeginLoad();

		assert(result == true);
		
		// Insert the object into the synchronization queue
		m_SynchronizationQueue.enqueue(loadData.object);
	}
}