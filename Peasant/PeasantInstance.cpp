////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantInstance.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PeasantInstance.h"
#include "PeasantObjectFactory.h"
#include "PeasantObjectManager.h"

// Using namespace Peasant
PeasantUsingDevelopmentNamespace(Peasant)

PeasantInstance::PeasantInstance()
{
	// Set the initial data
	m_IsLocked = true;
	m_DependencyCount = 0;
	m_LinkedInstanceDependency = nullptr;
	m_ReferenceObject = nullptr;
	m_Hash = PeasantHash();
	m_ObjectManager = nullptr;
	m_FactoryPtr = nullptr;
}

PeasantInstance::~PeasantInstance()
{
}

PeasantInstance::PeasantInstance(PeasantInstance&& _other)
{
	// Check if this instance is not being loaded or if it was completly loaded
	assert(!m_IsLocked && (!AreDependenciesFulfilled() || !WasLoaded()));

	// Set the data for this object
	m_IsLocked = _other.m_IsLocked;
	m_ReferenceObject = _other.m_ReferenceObject;
	m_DependencyCount.exchange(_other.m_DependencyCount);
	m_LinkedInstanceDependency = _other.m_LinkedInstanceDependency;
	m_Hash = _other.m_Hash;
	m_ObjectManager = _other.m_ObjectManager;
	m_FactoryPtr = _other.m_FactoryPtr;

	// Set the data for the other object
	m_IsLocked = true;
	m_ReferenceObject = nullptr;
	m_DependencyCount = 0;
	m_LinkedInstanceDependency = nullptr;
	m_Hash = PeasantHash();
	m_ObjectManager = nullptr;
	m_FactoryPtr = nullptr;
}

PeasantInstance& PeasantInstance::operator=(PeasantInstance&& _other)
{
	// Check if this instance isn't loaded
	assert(!(m_IsLocked || !AreDependenciesFulfilled() || !WasLoaded()));

	// We can't assign to the same object
	if (this != &_other)
	{
		// Set the data for this object
		m_IsLocked = _other.m_IsLocked;
		m_ReferenceObject = _other.m_ReferenceObject;
		m_DependencyCount.exchange(_other.m_DependencyCount);
		m_LinkedInstanceDependency = _other.m_LinkedInstanceDependency;
		m_Hash = _other.m_Hash;
		m_ObjectManager = _other.m_ObjectManager;
		m_FactoryPtr = _other.m_FactoryPtr;

		// Set the data for the other object
		m_IsLocked = true;
		m_ReferenceObject = nullptr;
		m_DependencyCount = 0;
		m_LinkedInstanceDependency = nullptr;
		m_Hash = PeasantHash();
		m_ObjectManager = nullptr;
		m_FactoryPtr = nullptr;
	}

	return *this;
}

void PeasantInstance::AddInstanceDependency(PeasantInstance& _instance)
{
	// Increment the dependency count
	m_DependencyCount++;

	// Set the instance dependency
	_instance.m_LinkedInstanceDependency = this;
}

PeasantObject* PeasantInstance::GetObjectPtr()
{
	assert(m_ReferenceObject != nullptr);
	return m_ReferenceObject;
}

void PeasantInstance::SetObjectReference(PeasantObject* _objectReference)
{
	m_ReferenceObject = _objectReference;
}

bool PeasantInstance::AreDependenciesFulfilled()
{
	return m_DependencyCount == 0;
}

bool PeasantInstance::RequestDuplicate(PeasantInstance& _other)
{
	// Check if this instance was completly loaded
	assert(m_IsLocked || !AreDependenciesFulfilled() || !WasLoaded());

	// Request a new object for this instance
	return m_ObjectManager->RequestObject(this, m_Hash, m_FactoryPtr, false);
}

bool PeasantInstance::WasLoaded()
{
	return m_ReferenceObject != nullptr && m_ReferenceObject->WasLoaded();
}

bool PeasantInstance::WasSynchronized()
{
	return m_ReferenceObject != nullptr && m_ReferenceObject->WasSynchronized();
}

bool PeasantInstance::IsLocked()
{
	return m_IsLocked;
}

void PeasantInstance::RegisterInfo(PeasantHash _hash, PeasantObjectManager* _manager)
{
	m_Hash = _hash;
	m_ObjectManager = _manager;
}

void PeasantInstance::Unlock()
{
	assert(m_IsLocked == true);
	assert(m_DependencyCount == 0);
	assert(m_ReferenceObject != nullptr);
	assert(m_ReferenceObject->WasLoaded());
	assert(m_ReferenceObject->WasSynchronized());

	m_IsLocked = false;
}

void PeasantInstance::BeginConstruction()
{
	assert(m_DependencyCount == 0);

	// Call the OnConstruct() method
	OnConstruct();

	// Check if this instance has any dependency
	if (m_DependencyCount == 0)
	{
		// Call the OnDependenciesFulfilled() method
		OnDependenciesFulfilled();

		// Unlock this instance
		Unlock();
	}

	// If some instance depends on this one
	if (m_LinkedInstanceDependency != nullptr)
	{
		// Fulfill the dependency
		m_LinkedInstanceDependency->FulfillDependency(this);
	}
}

void PeasantInstance::FulfillDependency(PeasantInstance* _instance)
{
	assert(m_DependencyCount != 0);

	// Subtract one from the dependency count
	m_DependencyCount--;

	// Check if we fulfilled all dependencies
	if (m_DependencyCount == 0)
	{
		// Call the OnDependenciesFulfilled() method
		OnDependenciesFulfilled();

		// Unlock this instance
		Unlock();
	}
}