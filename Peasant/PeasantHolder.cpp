////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantHolder.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PeasantHolder.h"

#include <cassert>

// Using namespace Peasant
PeasantUsingDevelopmentNamespace(Peasant)

PeasantHolder::PeasantHolder(PeasantObject* _object)
{
	// Set the initial data
	m_ReferenceObject = _object;
}

PeasantHolder::~PeasantHolder()
{
}