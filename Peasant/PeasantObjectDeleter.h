////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantObjectDeleter.h
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

// Delete method type
typedef std::function<bool(PeasantObject* _object)> ObjectDeleteMethod;

////////////////
// FORWARDING //
////////////////

////////////////////////////////////////////////////////////////////////////////
// Class name: PeasantObjectDeleter
////////////////////////////////////////////////////////////////////////////////
class PeasantObjectDeleter
{
public:

//////////////////
// CONSTRUCTORS //
public: //////////

	// Constructor / destructor
	PeasantObjectDeleter(ObjectDeleteMethod _objectDeleteMethod);
	~PeasantObjectDeleter();

//////////////////
// MAIN METHODS //
public: //////////

	// Load a new object
	bool DeleteObject(PeasantObject* _object);

private:

	// The auxiliar delete method
	void DeleteObjectAuxiliar();

///////////////
// VARIABLES //
private: //////

	// The auxiliar thread
	std::thread m_AuxiliarThread;

	// The object queue
	moodycamel::ReaderWriterQueue<PeasantObject*> m_Queue;

	// The delete method
	ObjectDeleteMethod m_DeleteMethod;
};

// Peasant
PeasantDevelopmentNamespaceEnd(Peasant)