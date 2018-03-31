////////////////////////////////////////////////////////////////////////////////
// Filename: PeonJob.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include <cstdint>

/////////////
// DEFINES //
/////////////

#define PeasantNamespaceBegin(name)					namespace name {
#define PeasantNamespaceEnd(name)					}

#define PeasantDevelopmentNamespaceBegin(name)		namespace __development__ ## name {
#define PeasantDevelopmentNamespaceEnd(name)		}

#define PeasantUsingNamespace(name)					using namespace name;
#define PeasantUsingDevelopmentNamespace(name)		using namespace __development__ ## name;

#define PeasantDevelopmentNamespace(name)			__development__ ## name

PeasantDevelopmentNamespaceBegin(Peasant)

template <typename ObjectType>
struct GlobalInstance
{
	// Constructor
	GlobalInstance()
	{
		// Create a new instance in case of nullptr
		if (m_InternalObject == nullptr)
		{
			m_InternalObject = new ObjectType();
		}
	}

	// Return the instance
	const static ObjectType* GetInstance()
	{
		return m_InternalObject;
	}

	// Access the internal object
	GlobalInstance operator=(ObjectType* _other)
	{
		m_InternalObject = _other;
		return *this;
	}

	operator ObjectType*()
	{
		return m_InternalObject;
	}

	// Access the internal object
	ObjectType* operator->() const
	{
		//m_iterator is a map<int, MyClass>::iterator in my code.
		return m_InternalObject;
	}

private:

	// The internal object
	static ObjectType* m_InternalObject;
};

template <typename ObjectType>
ObjectType* GlobalInstance<ObjectType>::m_InternalObject = nullptr;

// The peasant hash key type
typedef uint64_t PeasantHash;

PeasantDevelopmentNamespaceEnd(Peasant)
