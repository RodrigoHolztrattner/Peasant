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
#include "PeasantObjectFactory.h"

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

////////////////
// FORWARDING //
////////////////

////////////////////////////////////////////////////////////////////////////////
// Class name: PeasantObjectDeleter
////////////////////////////////////////////////////////////////////////////////
class PeasantObjectDeleter
{
public:

	// The delete request type
	struct DeleteRequest
	{
		// The object and factory
		PeasantObject* object;
		PeasantObjectFactory* factory;

		// If this object should be deleted synchronous
		bool deleteSync;
	};

	// The factory delete request
	struct FactoryDeleteRequest
	{
		// The object and factory
		PeasantObject* object;
		PeasantObjectFactory* factory;
	};

//////////////////
// CONSTRUCTORS //
public: //////////

	// Constructor / destructor
	PeasantObjectDeleter();
	~PeasantObjectDeleter();

//////////////////
// MAIN METHODS //
public: //////////

	// Load a new object
	bool DeleteObject(PeasantObject* _object, PeasantObjectFactory* _factoryPtr, bool _deleteSync);

	// The update method
	void Update();

private:

	// The auxiliar delete method
	void DeleteObjectAuxiliar();

///////////////
// VARIABLES //
private: //////

	// The auxiliar thread
	std::thread m_AuxiliarThread;

	// The object queue
	moodycamel::ReaderWriterQueue<DeleteRequest> m_Queue;

	// The factory deletion queue (used to delete the object itself by the factory in sync mode)
	moodycamel::ReaderWriterQueue<FactoryDeleteRequest> m_FactoryDeletionQueue;
};

// Peasant
PeasantDevelopmentNamespaceEnd(Peasant)