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

bool PeasantSystem::RequestObject(PeasantInstance& _instance, PeasantHash _hash, PeasantObjectFactory* _factoryPtr, bool _allowAsynchronousConstruct)
{
	return m_ObjectManager.RequestObject(&_instance, _hash, _factoryPtr, _allowAsynchronousConstruct);
}

void PeasantSystem::ReleaseObject(PeasantInstance& _instance, PeasantObjectFactory* _factoryPtr, bool _allowAsynchronousDeletion)
{
	m_ObjectManager.ReleaseObject(&_instance, _factoryPtr, _allowAsynchronousDeletion);
}

void PeasantSystem::Update()
{
	m_ObjectManager.Update();
}