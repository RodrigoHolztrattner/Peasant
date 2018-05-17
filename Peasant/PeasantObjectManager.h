////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantObjectManager.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "PeasantConfig.h"
#include "PeasantObject.h"
#include "PeasantStorage.h"
#include "PeasantObjectFactory.h"
#include "PeasantMultipleQueue.h"
#include "PeasantObjectLoader.h"
#include "PeasantObjectDeleter.h"

#include <cstdint>
#include <vector>

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

////////////////////////////////////////////////////////////////////////////////
// Class name: PeasantObjectManager
////////////////////////////////////////////////////////////////////////////////
class PeasantObjectManager
{
public:

	// The request type
	struct ObjectRequest
	{
		// The instance and hash variables
		PeasantInstance* instance;
		PeasantHash hash;

		// The factory ptr
		PeasantObjectFactory* factoryPtr;

		// If the object is permanent
		bool isPermanent;
	};

	// The release type
	struct ObjectRelease
	{
		// The object
		PeasantObject* object;

		// The factory ptr
		PeasantObjectFactory* factoryPtr;

		// If this object should be deleted synchronous
		bool deleteSync;
	};

	// The temporary object release
	struct TemporaryObjectRelease
	{
		// The object
		PeasantObject* object;

		// The factory ptr
		PeasantObjectFactory* factoryPtr;

		// If this object should be deleted synchronous
		bool deleteSync;
	};

//////////////////
// CONSTRUCTORS //
public: //////////

	// Constructor / destructor
	PeasantObjectManager(PeasantStorage& _storageReference, uint32_t _workerThreads, ThreadIndexRetrieveMethod _threadIndexMethod, ObjectLoadMethod _objectLoadMethod);
	~PeasantObjectManager();
	
//////////////////
// MAIN METHODS //
public: //////////

	// Request an object for the given instance and resource hash
	bool RequestObject(PeasantInstance* _instance, PeasantHash _hash, PeasantObjectFactory* _factoryPtr, bool _allowAsynchronousConstruct = false);

	// Request a permanent object for the given instance and resource hash, the object will not be deleted when it reaches 0
	// references, the deletion phase will only occur in conjunction with the storage deletion
	bool RequestPersistentObject(PeasantInstance* _instance, PeasantHash _hash, PeasantObjectFactory* _factoryPtr, bool _allowAsynchronousConstruct = false);

	// Release an object instance
	void ReleaseObject(PeasantInstance* _instance, PeasantObjectFactory* _factoryPtr, bool _allowAsynchronousDeletion = false);
	void ReleaseObject(PeasantObject* _object, PeasantObjectFactory* _factoryPtr, bool _allowAsynchronousDeletion = false);

	// The update method, process all requests
	void Update();

///////////////
// VARIABLES //
private: //////

	// The mutex we will use to secure thread safety
	std::mutex m_Mutex;

	// The object loader and deleter
	PeasantObjectLoader m_ObjectLoader;
	PeasantObjectDeleter m_ObjectDeleter;

	// The object requests and the release queue
	MultipleQueue<ObjectRequest> m_ObjectRequests;
	MultipleQueue<ObjectRelease> m_InstanceReleases;
	MultipleQueue<TemporaryObjectRelease> m_TemporaryInstanceReleases;

	// The construct queue
	std::vector<PeasantInstance*> m_ConstructQueue;

	// The object storage reference
	PeasantStorage& m_StorageReference;
	
	// If we are inside the update phase and the updating thread id (used for asserts only)
	bool m_InUpdatePhase;
	std::thread::id m_UpdateThreadID;
};

// Peasant
PeasantDevelopmentNamespaceEnd(Peasant)