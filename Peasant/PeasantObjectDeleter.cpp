////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantObjectDeleter.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PeasantObjectDeleter.h"
#include <cassert>
#include <chrono>

// Using namespace Peasant
PeasantUsingDevelopmentNamespace(Peasant)

PeasantObjectDeleter::PeasantObjectDeleter(ObjectDeleteMethod _objectDeleteMethod)
{
	// Set the initial data
	m_DeleteMethod = _objectDeleteMethod;

	// Create the auxiliar thread
	m_AuxiliarThread = std::thread(&PeasantObjectDeleter::DeleteObjectAuxiliar, this);
}

PeasantObjectDeleter::~PeasantObjectDeleter()
{
}

bool PeasantObjectDeleter::DeleteObject(PeasantObject* _object)
{
	return m_Queue.enqueue(_object);
}

void PeasantObjectDeleter::DeleteObjectAuxiliar()
{
	// Do forever
	while (true)
	{
		// Try to get an object from the queue
		PeasantObject* object;
		if (!m_Queue.try_dequeue(object))
		{
			// Sleep because there is no object to dequeue
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			continue;
		}

		// Delete this object
		bool result = m_DeleteMethod(object);
		assert(result == true);
	}
}