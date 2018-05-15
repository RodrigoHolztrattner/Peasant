////////////////////////////////////////////////////////////////////////////////
// Filename: PeonJob.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include <cstdint>
#include <string>

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

// Hash the given str (static)
static constexpr uint64_t HashString(const char* _str)
{
	return *_str ?
		static_cast<uint64_t>(*_str) + 33 * HashString(_str + 1) :
		5381;
}

// Hash the given str (non static)
static uint64_t HashString(char* _str)
{
	return *_str ?
		static_cast<uint64_t>(*_str) + 33 * HashString(_str + 1) :
		5381;
}

// The peasant hash key type
struct PeasantHash
{
	// Default constructor
	PeasantHash()
	{
		// Set the initial data
		hash = flags = 0;
	}

	// Construct with input
	PeasantHash(const char* _str, uint64_t _flags = 0) : hash(HashString(_str)), flags(_flags) {}
	PeasantHash(char* _str, uint64_t _flags = 0) : hash(HashString(_str)), flags(_flags) {}
	PeasantHash(const std::string _str, uint64_t _flags = 0) : hash(HashString(_str.c_str())), flags(_flags) {}
	PeasantHash(uint64_t _hash) : hash(_hash), flags(0) {}

	// Compare operator
	bool operator<(const PeasantHash& _other) const
	{
		// If both hashes are equal, compare the flags
		if (hash == _other.hash)
		{
			return flags < _other.flags;
		}
		return hash < _other. hash;
	}

	// The hash value
	uint64_t hash;

	// The flag value
	uint64_t flags;
};

PeasantDevelopmentNamespaceEnd(Peasant)
