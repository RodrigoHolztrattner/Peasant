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
	m_DataValid = false;
	m_WasSynchronized = false;
	m_TotalReferences = 0;
	m_Data = nullptr;
	m_DataSize = 0;
}

PeasantObject::~PeasantObject()
{
	assert(m_TotalReferences == 0);
}

bool PeasantObject::BeginLoad()
{
	// Check if the data is valid
	if (m_Data == nullptr || m_DataSize == 0)
	{
		return false;
	}

	// Call the Onload() method
	if (!OnLoad(m_Data, m_DataSize))
	{
		return false;
	}

	// Set data valid and loaded
	m_DataValid = true;
	m_IsLoaded = true;

	return true;
}

bool PeasantObject::BeginDelete()
{
	// Check if we have some data to delete
	if (m_Data == nullptr || m_DataSize == 0)
	{
		return false;
	}

	// Set unloaded
	m_IsLoaded = false;

	// Call the OnDelete() method
	if (!OnDelete(m_Data))
	{
		return false;
	}

	// Check if the data was deleted
	if (m_Data != nullptr)
	{
		return false;
	}

	// Set the data size
	m_DataSize = 0;

	// Unsynchronize this object
	m_WasSynchronized = false;

	// Set data invalid
	m_DataValid = false;

	return true;
}

bool PeasantObject::BeginSynchronization()
{
	assert(!m_WasSynchronized);

	// Call the OnSynchronization() method
	OnSynchronization();

	// Set synchronized
	m_WasSynchronized = true;

	return true;
}

bool PeasantObject::WasLoaded()
{
	return m_IsLoaded;
}

bool PeasantObject::IsReferenced()
{
	return m_TotalReferences > 0;
}

bool PeasantObject::IsDataValid()
{
	return m_DataValid;
}

bool PeasantObject::WasSynchronized()
{
	return m_WasSynchronized;
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

uint32_t PeasantObject::GetDataSize()
{
	return m_DataSize;
}

uint32_t& PeasantObject::GetDataSizeRef()
{
	return m_DataSize;
}

unsigned char** PeasantObject::GetDataPtrRef()
{
	return &m_Data;
}