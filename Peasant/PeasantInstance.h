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
class PeasantObjectFactory;

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

	// Move constructor (this doesnt' copy parent member variables, do this on your own)
	PeasantInstance(PeasantInstance&& _other);

	// Move assignment constructor (this doesnt' copy parent member variables, do this on your own)
	PeasantInstance& operator=(PeasantInstance&& _other);

//////////////////
// MAIN METHODS //
public: //////////

	// Return if this object is locked
	bool IsLocked();

	// Return if this internal object was loaded
	bool WasLoaded();

	// Return if the internal object was synchronized
	bool WasSynchronized();

	// Return if all dependencies are fulfilled
	bool AreDependenciesFulfilled();

	// Request a duplicated reference for this instance (this doesnt' duplicate parent member variables, do this on your own)
	bool RequestDuplicate(PeasantInstance& _other);

protected:

	// Add a dependency to another instance
	void AddInstanceDependency(PeasantInstance& _instance);

	// Return the object ptr
	PeasantObject* GetObjectPtr();

	// Return the object factory casting to the given template typeclass
	template<typename FactoryClass>
	FactoryClass* GetFactoryPtr()
	{
		return m_ReferenceObject->GetFactoryPtr<FactoryClass>();
	}

protected:

	// Register the hash and the manager for this instance
	void RegisterInfo(PeasantHash _hash, PeasantObjectManager* _manager);

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

	// The hash object
	PeasantHash m_Hash;

	// The object manager reference
	PeasantObjectManager* m_ObjectManager;

	// The factory ptr
	PeasantObjectFactory* m_FactoryPtr;
};

// Peasant
PeasantDevelopmentNamespaceEnd(Peasant)