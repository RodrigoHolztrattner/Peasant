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
		bool result = m_LoadMethod(loadData.object, loadData.hash);
		assert(result == true);
	}
}