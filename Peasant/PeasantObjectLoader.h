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
typedef std::function<bool(unsigned char* _dataPtr, uint32_t& _dataSizeRef, PeasantHash _hash)> ObjectLoadMethod;

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
		// The object and the hash
		PeasantObject* object;
		PeasantHash hash;
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
	bool LoadObject(PeasantObject* _object, PeasantHash _hash);

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

	// The load method
	ObjectLoadMethod m_LoadMethod;
};

// Peasant
PeasantDevelopmentNamespaceEnd(Peasant)