////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantSystem.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "PeasantConfig.h"
#include "PeasantObjectManager.h"
#include "PeasantStorage.h"

#include <cstdint>
#include <functional>

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

/////////////
// STRUCTS //
/////////////

// The config data
struct PeasantConfigData
{
	// The object load method
	ObjectLoadMethod												objectLoadMethod;

	// The total number of worker threads
	uint32_t														totalWorkerThreads;

	// The thread index retrieve method
	ThreadIndexRetrieveMethod										threadIndexRetrieveMethod;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: PeasantSystem
////////////////////////////////////////////////////////////////////////////////
class PeasantSystem
{
public:

//////////////////
// CONSTRUCTORS //
public: //////////

	// Constructor / destructor
	PeasantSystem(PeasantConfigData _configData);
	~PeasantSystem();

//////////////////
// MAIN METHODS //
public: //////////

	// Request an object for the given instance and resource hash
	bool RequestObject(PeasantInstance& _instance, PeasantHash _hash, PeasantObjectFactory* _factoryPtr);

	// Release an object instance
	void ReleaseObject(PeasantInstance& _instance, PeasantObjectFactory* _factoryPtr);

	// The update method, process all requests
	void Update();

///////////////
// VARIABLES //
private: //////

	// The object manager
	PeasantObjectManager m_ObjectManager;

	// The object storage
	PeasantStorage m_ObjectStorage;
};

// Peasant
PeasantDevelopmentNamespaceEnd(Peasant)