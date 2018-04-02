////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantInstance.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "PeasantConfig.h"
#include "PeasantObject.h"

#include <cstdint>
#include <vector>
#include <atomic>

///////////////
// NAMESPACE //
///////////////

/////////////
// DEFINES //
/////////////

////////////
// GLOBAL //
////////////

///////////////
// NAMESPACE //
///////////////

// Peasant
PeasantDevelopmentNamespaceBegin(Peasant)

//////////////
// TYPEDEFS //
//////////////

////////////////
// FORWARDING //
////////////////

// Classes we know
class PeasantObjectManager;
class PeasantObject;

////////////////////////////////////////////////////////////////////////////////
// Class name: PeasantInstance
////////////////////////////////////////////////////////////////////////////////
class PeasantInstance
{
public:

	// Friend classes
	friend PeasantObjectManager;
	friend PeasantObject;

//////////////////
// CONSTRUCTORS //
public: //////////

	// Constructor / destructor
	PeasantInstance();
	PeasantInstance(const PeasantInstance& _other) = delete;
	virtual ~PeasantInstance();

//////////////////
// MAIN METHODS //
public: //////////

	// Add a dependency to another instance
	void AddInstanceDependency(PeasantInstance& _instance);

	// Return if this object is locked
	bool IsLocked();

	// Return if this internal object was loaded
	bool WasLoaded();

	// Return if the internal object was synchronized
	bool WasSynchronized();

	// Return if all dependencies are fulfilled
	bool AreDependenciesFulfilled();

	// Return the object ptr
	PeasantObject* GetObjectPtr();

protected:

	// Unlock this instance to be used by the world
	void Unlock();

	// Begin the construction of this instance
	void BeginConstruction();

	// Set the peasant object reference
	void SetObjectReference(PeasantObject* _objectReference);

/////////////////////
// VIRTUAL METHODS //
protected: //////////

	// The on xxx methods
	virtual void OnConstruct() = 0;
	virtual void OnDependenciesFulfilled() = 0;

private:

	// Fulfill a dependency
	void FulfillDependency(PeasantInstance* _instance);

///////////////
// VARIABLES //
private: //////

	// If this instance is locked
	bool m_IsLocked;

	// The object we are referencing
	PeasantObject* m_ReferenceObject;

	// The dependency count
	std::atomic<uint32_t> m_DependencyCount;

	// The instance that depends on this one
	PeasantInstance* m_LinkedInstanceDependency;
};

// Peasant
PeasantDevelopmentNamespaceEnd(Peasant)