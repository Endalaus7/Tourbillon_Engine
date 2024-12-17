#include "ECSManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "RenderSystem/render/render_system.h"
#include "common.hpp"

void TourBillon::ECSManager::initialize(ECSManagerInitInfo ECS_Manager_init_info)
{
	mComponentManager = std::make_unique<ComponentManager>();
	mEntityManager = std::make_unique<EntityManager>();
	//mEventManager = std::make_unique<EventManager>();
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

void TourBillon::ECSManager::run()
{
	//m_render_thread = std::thread(&RenderSystem::rendLoop, m_renderSystem);
	//m_render_thread.join();
	
	

	//

	renderShouldClose = true;
}

