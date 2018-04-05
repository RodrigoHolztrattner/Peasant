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
class PeasantObjectFactory;

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

	// The OnLoad method (asynchronous method)
	virtual bool OnLoad(unsigned char* _data, uint32_t _dataSize) = 0;

	// The OnSynchronization method (synchronous method)
	virtual bool OnSynchronization() = 0;

	// The OnDelete() method (asynchronous method)
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

	// Return if this object was synchronized
	bool WasSynchronized();

	// Return the object hash
	PeasantHash GetHash();

	// Return the object factory casting to the given template typeclass
	template<typename FactoryClass>
	FactoryClass* GetFactoryPtr()
	{
		return reinterpret_cast<FactoryClass*>(m_Factory);
	}

protected:

	// Begin load, deletion and synchronize methods
	bool BeginLoad();
	bool BeginDelete();
	bool BeginSynchronization();

	// Set the hash
	void SetHash(PeasantHash _hash);

	// Set the factory reference
	void SetFactoryReference(PeasantObjectFactory* _factoryReference);

	// Make a instance reference this object
	void MakeInstanceReference(PeasantInstance* _instance);

	// Release a instance of this object
	void ReleaseInstance(PeasantInstance* _instance);

	// Return the data size and ptr
	uint32_t GetDataSize();
	uint32_t& GetDataSizeRef();
	unsigned char** GetDataPtrRef();

///////////////
// VARIABLES //
private: //////

	// If this object was loaded, if the data is valid and if this object was synchronized
	bool m_IsLoaded;
	bool m_DataValid;
	bool m_WasSynchronized;

	// The total number of references
	std::atomic<uint32_t> m_TotalReferences;

	// The object hash
	PeasantHash m_Hash;

	// The object data
	unsigned char* m_Data;

	// The object data size
	uint32_t m_DataSize;

	// The object factory
	PeasantObjectFactory* m_Factory;
};

// Peasant
PeasantDevelopmentNamespaceEnd(Peasant)