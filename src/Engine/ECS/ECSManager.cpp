#include "ECSManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "RenderSystem/render_system.h"
#include "common.hpp"

void TourBillon::ECSManager::initialize(ECSManagerInitInfo ECS_Manager_init_info)
{
	mComponentManager = std::make_unique<ComponentManager>();
	mEntityManager = std::make_unique<EntityManager>();
	mEventManager = std::make_unique<EventManager>();
	mSystemManager = std::make_unique<SystemManager>();

}

TourBillon::Entity TourBillon::ECSManager::CreateEntity()
{
	return mEntityManager->CreateEntity();
}

void TourBillon::ECSManager::DestroyEntity(TourBillon::Entity entity)
{
	mEntityManager->DestroyEntity(entity);

	mComponentManager->EntityDestroyed(entity);

	mSystemManager->EntityDestroyed(entity);
}

void TourBillon::ECSManager::AddListener(Events::EventType type, std::function<void(const CEvent&)> callback)
{
	mEventManager->AddListener(type, callback);
}

void TourBillon::ECSManager::SendEvent(Events::EventType type, void* data, size_t datasize)
{
	mEventManager->SendEvent(type, data, datasize);
}
