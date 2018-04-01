////////////////////////////////////////////////////////////////////////////////
// Filename: PeasantSystem.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PeasantSystem.h"

// Using namespace Peasant
PeasantUsingDevelopmentNamespace(Peasant)

PeasantSystem::PeasantSystem(PeasantConfigData _configData) : 
	m_ObjectManager(m_ObjectStorage, _configData.totalWorkerThreads, _configData.threadIndexRetrieveMethod, _configData.objectLoadMethod)
{
	// Set the initial data
	// ...
}

PeasantSystem::~PeasantSystem()
{
}

bool PeasantSystem::RequestObject(PeasantInstance& _instance, PeasantHash _hash, PeasantObjectFactory* _factoryPtr)
{
	return m_ObjectManager.RequestObject(&_instance, _hash, _factoryPtr);
}

void PeasantSystem::ReleaseObject(PeasantInstance& _instance, PeasantObjectFactory* _factoryPtr)
{
	m_ObjectManager.ReleaseObject(&_instance, _factoryPtr);
}

void PeasantSystem::Update()
{
	m_ObjectManager.Update();
}