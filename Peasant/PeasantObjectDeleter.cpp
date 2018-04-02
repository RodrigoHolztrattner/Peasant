////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantObjectDeleter.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PeasantObjectDeleter.h"
#include <cassert>
#include <chrono>

// Using namespace Peasant
PeasantUsingDevelopmentNamespace(Peasant)

PeasantObjectDeleter::PeasantObjectDeleter()
{
	// Create the auxiliar thread
	m_AuxiliarThread = std::thread(&PeasantObjectDeleter::DeleteObjectAuxiliar, this);
}

PeasantObjectDeleter::~PeasantObjectDeleter()
{
}

bool PeasantObjectDeleter::DeleteObject(PeasantObject* _object, PeasantObjectFactory* _factoryPtr, bool _deleteSync)
{
	// Create the delete request
	DeleteRequest deleteRequest = { _object, _factoryPtr, _deleteSync };

	return m_Queue.enqueue(deleteRequest);
}

void PeasantObjectDeleter::Update()
{
	// For each object inside our factory deletion queue...
	FactoryDeleteRequest factoryDeleteRequest;
	while (m_FactoryDeletionQueue.try_dequeue(factoryDeleteRequest))
	{
		assert(!factoryDeleteRequest.object->IsDataValid());
		assert(!factoryDeleteRequest.object->IsReferenced());
		assert(!factoryDeleteRequest.object->WasLoaded());

		// Call the release method for the factory
		factoryDeleteRequest.factory->ReleaseObject(factoryDeleteRequest.object);
	}
}

void PeasantObjectDeleter::DeleteObjectAuxiliar()
{
	// Do forever
	while (true)
	{
		// Try to get an object from the queue
		DeleteRequest deleteRequest;
		if (!m_Queue.try_dequeue(deleteRequest))
		{
			// Sleep because there is no object to dequeue
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			continue;
		}

		// Call the OnDelete() method for this object (to release the data)
		bool result = deleteRequest.object->BeginDelete();

		assert(result == true);

		// Check if the factory should delete this object synchronous
		if (!deleteRequest.deleteSync)
		{
			assert(!deleteRequest.object->IsDataValid());
			assert(!deleteRequest.object->IsReferenced());
			assert(!deleteRequest.object->WasLoaded());

			// Call the release method for the factory assynchronous
			deleteRequest.factory->ReleaseObject(deleteRequest.object);
		}
		else
		{
			// Create a new factory deletion request
			FactoryDeleteRequest factoryDeleteRequest = { deleteRequest.object, deleteRequest.factory };

			// Insert into the factory deletion request
			m_FactoryDeletionQueue.enqueue(factoryDeleteRequest);
		}
	}
}