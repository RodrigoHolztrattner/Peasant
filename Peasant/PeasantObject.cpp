////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantObject.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PeasantObject.h"
#include "PeasantInstance.h"

#include <cassert>

// Using namespace Peasant
PeasantUsingDevelopmentNamespace(Peasant)

PeasantObject::PeasantObject()
{
	// Set the initial data
	m_IsLoaded = true;
	m_TotalReferences = 0;
}

PeasantObject::~PeasantObject()
{
	assert(m_TotalReferences == 0);
}

bool PeasantObject::WasLoaded()
{
	return m_IsLoaded;
}

bool PeasantObject::IsReferenced()
{
	return m_TotalReferences > 0;
}

PeasantHash PeasantObject::GetHash()
{
	return m_Hash;
}

void PeasantObject::SetHash(PeasantHash _hash)
{
	m_Hash = _hash;
}

void PeasantObject::MakeInstanceReference(PeasantInstance* _instance)
{
	// Increment the total number of references
	m_TotalReferences++;

	// Set the object reference
	_instance->SetObjectReference(this);
}

void PeasantObject::ReleaseInstance(PeasantInstance* _instance)
{
	assert(m_TotalReferences > 0);

	// Subtract one from the reference count
	m_TotalReferences--;
}