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

bool PeasantObjectDeleter::DeleteObject(PeasantObject* _object, PeasantObjectFactory* _factoryPtr)
{
	// Create the delete request
	DeleteRequest deleteRequest = { _object, _factoryPtr };

	return m_Queue.enqueue(deleteRequest);
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

		// Call the release method for the factory
		deleteRequest.factory->ReleaseObject(deleteRequest.object); // TODO remove from here and make this sync with the manager

		assert(result == true);
	}
}