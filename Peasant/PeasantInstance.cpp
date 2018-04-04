////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantInstance.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PeasantInstance.h"
#include "PeasantObjectFactory.h"

#include <cassert>

// Using namespace Peasant
PeasantUsingDevelopmentNamespace(Peasant)

PeasantInstance::PeasantInstance()
{
	// Set the initial data
	m_IsLocked = true;
	m_DependencyCount = 0;
	m_LinkedInstanceDependency = nullptr;
	m_ReferenceObject = nullptr;
}

PeasantInstance::~PeasantInstance()
{
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

bool PeasantInstance::WasLoaded()
{
	return m_ReferenceObject->WasLoaded();
}

bool PeasantInstance::WasSynchronized()
{
	return m_ReferenceObject->WasSynchronized();
}

bool PeasantInstance::IsLocked()
{
	return m_IsLocked;
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