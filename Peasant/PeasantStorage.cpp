////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantStorage.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PeasantStorage.h"
#include "PeasantObject.h"

#include <cassert>

// Using namespace Peasant
PeasantUsingDevelopmentNamespace(Peasant)

PeasantStorage::PeasantStorage()
{
	// Set the initial data
	// ...
}

PeasantStorage::~PeasantStorage()
{
}

PeasantObject* PeasantStorage::FindObject(PeasantHash _hash)
{
	// Check if an object with the given hash exist
	auto iterator = m_ObjectMap.find(_hash);
	if (iterator == m_ObjectMap.end())
	{
		return nullptr;
	}

	return iterator->second;
}

bool PeasantStorage::InsertObject(PeasantObject* _object, PeasantHash _hash)
{
	// Check if an object with the given hash exist
	auto iterator = m_ObjectMap.find(_hash);
	if (iterator != m_ObjectMap.end())
	{
		return false;
	}

	// Insert the object
	m_ObjectMap.insert(std::pair<PeasantHash, PeasantObject*>(_hash, _object));

	return true;
}

bool PeasantStorage::RemoveObject(PeasantObject* _object)
{
	return RemoveObject(_object->GetHash());
}

bool PeasantStorage::RemoveObject(PeasantHash _hash)
{
	// Check if an object with the given hash exist
	auto iterator = m_ObjectMap.find(_hash);
	if (iterator == m_ObjectMap.end())
	{
		return false;
	}

	// Erase the current object on the iterator
	m_ObjectMap.erase(iterator);

	return true;
}