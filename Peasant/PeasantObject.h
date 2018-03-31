////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantObject.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "PeasantConfig.h"

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
class PeasantInstance;
class PeasantObjectManager;

////////////////////////////////////////////////////////////////////////////////
// Class name: PeasantObject
////////////////////////////////////////////////////////////////////////////////
class PeasantObject
{
public:

	// Friend classes
	friend PeasantObjectManager;

//////////////////
// CONSTRUCTORS //
public: //////////

	// Constructor / destructor
	PeasantObject();
	virtual ~PeasantObject();

//////////////////
// MAIN METHODS //
public: //////////

	// Return if this object was loaded
	bool WasLoaded();

	// Return if this object is referenced
	bool IsReferenced();

	// Return the object hash
	PeasantHash GetHash();

protected:

	// Set the hash
	void SetHash(PeasantHash _hash);

	// Make a instance reference this object
	void MakeInstanceReference(PeasantInstance* _instance);

	// Release a instance of this object
	void ReleaseInstance(PeasantInstance* _instance);

///////////////
// VARIABLES //
private: //////

	// If this object was loaded
	bool m_IsLoaded;

	// The total number of references
	std::atomic<uint32_t> m_TotalReferences;

	// The object hash
	PeasantHash m_Hash;
};

// Peasant
PeasantDevelopmentNamespaceEnd(Peasant)