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
class PeasantObjectLoader;
class PeasantObjectDeleter;

////////////////////////////////////////////////////////////////////////////////
// Class name: PeasantObject
////////////////////////////////////////////////////////////////////////////////
class PeasantObject
{
public:

	// Friend classes
	friend PeasantObjectManager;
	friend PeasantObjectLoader;
	friend PeasantObjectDeleter;

//////////////////
// CONSTRUCTORS //
public: //////////

	// Constructor / destructor
	PeasantObject();
	virtual ~PeasantObject();

/////////////////////
// VIRTUAL METHODS //
protected: //////////

	// The OnLoad method
	virtual bool OnLoad(unsigned char* _data, uint32_t _dataSize) = 0;

	// The OnDelete() method
	virtual bool OnDelete(unsigned char* _data) = 0;

//////////////////
// MAIN METHODS //
public: //////////

	// Return if this object was loaded
	bool WasLoaded();

	// Return if this object is referenced
	bool IsReferenced();

	// Return if the data is valid
	bool IsDataValid();

	// Return the object hash
	PeasantHash GetHash();

protected:

	// Begin load and deletion methods
	bool BeginLoad();
	bool BeginDelete();

	// Set the hash
	void SetHash(PeasantHash _hash);

	// Make a instance reference this object
	void MakeInstanceReference(PeasantInstance* _instance);

	// Release a instance of this object
	void ReleaseInstance(PeasantInstance* _instance);

	// Return the data size and ptr
	uint32_t GetDataSize();
	uint32_t& GetDataSizeRef();
	unsigned char* GetDataPtr();

///////////////
// VARIABLES //
private: //////

	// If this object was loaded and if the data is valid
	bool m_IsLoaded;
	bool m_DataValid;

	// The total number of references
	std::atomic<uint32_t> m_TotalReferences;

	// The object hash
	PeasantHash m_Hash;

	// The object data
	unsigned char* m_Data;

	// The object data size
	uint32_t m_DataSize;
};

// Peasant
PeasantDevelopmentNamespaceEnd(Peasant)