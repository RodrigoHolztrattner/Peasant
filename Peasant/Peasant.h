////////////////////////////////////////////////////////////////////////////////
// Filename: Peasant.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "PeasantConfig.h"
#include "PeasantObject.h"
#include "PeasantInstance.h"
#include "PeasantObjectFactory.h"
#include "PeasantStorage.h"
#include "PeasantSystem.h"

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

template <typename PeasantObjectType>
using TemporaryInstance = PeasantDevelopmentNamespace(Peasant)::PeasantTemporaryInstance<PeasantObjectType>;

////////////////
// FORWARDING //
////////////////

// Peasant
PeasantNamespaceEnd(Peasant)