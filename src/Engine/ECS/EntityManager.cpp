#include "EntityManager.h"

TourBillon::EntityManager::EntityManager()
{
	Init();
}

void TourBillon::EntityManager::Init()
{
	for (Entity entity = 0; entity <= MAX_ENTITIES; ++entity)
	{
		m_entityPool.push(entity);
	}
}

TourBillon::Entity TourBillon::EntityManager::CreateEntity()
{
	if (m_currEntitiesNum >= MAX_ENTITIES)
	{
		LOG_WARNING("too many entities");
	}

	Entity id = m_entityPool.front();
	m_entityPool.pop();
	++m_currEntitiesNum;

	return id;
}

void TourBillon::EntityManager::DestroyEntity(Entity entity)
{
	if (m_currEntitiesNum <= entity)
	{
		LOG_ERROR("out of range");
	}

	mSignatures[entity].reset();
	m_entityPool.push(entity);
	--m_currEntitiesNum;
}

void TourBillon::EntityManager::SetSignature(Entity entity, Signature signature)
{
	if (m_currEntitiesNum <= entity)
	{
		LOG_ERROR("out of range");
	}

	mSignatures[entity] = signature;
}

TourBillon::Signature TourBillon::EntityManager::GetSignature(Entity entity)
{
	if (m_currEntitiesNum <= entity)
	{
		LOG_ERROR("out of range");
	}

	return mSignatures[entity];
}