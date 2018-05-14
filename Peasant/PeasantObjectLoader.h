////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantObjectLoader.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "PeasantConfig.h"
#include "PeasantObject.h"
#include "PeasantStorage.h"

#include "ThirdParty\readerwriterqueue\readerwriterqueue.h"
#include <thread>

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

// Load method type
typedef std::function<bool(unsigned char** _dataPtrRef, uint32_t& _dataSizeRef, PeasantHash _hash)> ObjectLoadMethod;

////////////////
// FORWARDING //
////////////////

////////////////////////////////////////////////////////////////////////////////
// Class name: PeasantObjectLoader
////////////////////////////////////////////////////////////////////////////////
class PeasantObjectLoader
{
public:

	// The load data
	struct LoadData
	{
		// The object, the hash and if it is permanent
		PeasantObject* object;
		PeasantHash hash;
		bool isPermanent;
	};

//////////////////
// CONSTRUCTORS //
public: //////////

	// Constructor / destructor
	PeasantObjectLoader(ObjectLoadMethod _objectLoadMethod);
	~PeasantObjectLoader();

//////////////////
// MAIN METHODS //
public: //////////

	// Load a new object
	bool LoadObject(PeasantObject* _object, PeasantHash _hash, bool _isPermanent);

	// The update method
	void Update();

private:

	// The auxiliar load method
	void LoadObjectAuxiliar();

///////////////
// VARIABLES //
private: //////

	// The auxiliar thread
	std::thread m_AuxiliarThread;

	// The object queue
	moodycamel::ReaderWriterQueue<LoadData> m_Queue;

	// The object synchronization queue (callable only if the object was loaded)
	moodycamel::ReaderWriterQueue<PeasantObject*> m_SynchronizationQueue;

	// The load method
	ObjectLoadMethod m_LoadMethod;
};

// Peasant
PeasantDevelopmentNamespaceEnd(Peasant)