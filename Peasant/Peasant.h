////////////////////////////////////////////////////////////////////////////////
// Filename: Peasant.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "PeasantConfig.h"
#include "PeasantSystem.h"
#include "PeasantObject.h"

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
PeasantNamespaceBegin(Peasant)

//////////////
// TYPEDEFS //
//////////////

typedef PeasantDevelopmentNamespace(Peasant)::PeasantObject				Object;
typedef PeasantDevelopmentNamespace(Peasant)::PeasantInstance			ObjectInstance;
typedef PeasantDevelopmentNamespace(Peasant)::PeasantObjectFactory		ObjectFactory;
typedef PeasantDevelopmentNamespace(Peasant)::PeasantSystem				System;
typedef PeasantDevelopmentNamespace(Peasant)::PeasantConfigData			ConfigData;
typedef PeasantDevelopmentNamespace(Peasant)::PeasantHash				Hash;

////////////////
// FORWARDING //
////////////////

// Peasant
PeasantNamespaceEnd(Peasant)