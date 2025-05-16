#pragma once

#include <memory>
#include <thread>
#include <chrono>
#include "ECSCommon.h"

#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "Assets.h"
//Coordinator


namespace TourBillon
{
	class RenderSystem;
	class ComponentManager;
	class EntityManager;
	class SystemManager;
	struct ECSManagerInitInfo
	{

	};
	class ECSManager:public Singleton<ECSManager>
	{
	public:
		virtual void initialize(ECSManagerInitInfo ECS_Manager_init_info);

		// Entity methods
		Entity CreateEntity();
		void DestroyEntity(Entity entity);

		// Component methods
		template<typename T>
		void RegisterComponent(){
			mComponentManager->RegisterComponent<T>();
		}
		template<typename T>
		void AddComponent(Entity entity, T& component)
		{
			mComponentManager->AddComponent<T>(entity, component);
			auto signature = mEntityManager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), true);
			mEntityManager->SetSignature(entity, signature);
			mSystemManager->EntitySignatureChanged(entity, signature);
		}
		template<typename T>
		void RemoveComponent(Entity entity)
		{
			mComponentManager->RemoveComponent<T>(entity);

			auto signature = mEntityManager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), false);
			mEntityManager->SetSignature(entity, signature);

			mSystemManager->EntitySignatureChanged(entity, signature);
		}
		template<typename T>
		T& GetComponent(Entity entity)
		{
			return mComponentManager->GetComponent<T>(entity);
		}
		template<typename T>
		size_t GetComponentSize()//有多少个组件的实体
		{
			return mComponentManager->GetComponentArraySize<T>();
		}

		template<typename T>
		TBVector<Entity> GetComponentEntities()
		{
			return mComponentManager->GetComponentEntities<T>();
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return mComponentManager->GetComponentType<T>();
		}
		// System methods
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			return mSystemManager->RegisterSystem<T>();
		}

		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			mSystemManager->SetSignature<T>(signature);
		}

		//需要注意add顺序，先add的先执行
		void AddListener(Events::EventType type, std::function<void(const CEvent&)> callback);
		void SendEvent(Events::EventType type, void* data = nullptr, size_t datasize = 0);



	public:
		std::unique_ptr<ComponentManager> mComponentManager;
		std::unique_ptr<EntityManager> mEntityManager;
		std::unique_ptr<EventManager> mEventManager;
		std::unique_ptr<SystemManager> mSystemManager;


		std::atomic<bool> renderShouldClose = false;

		std::thread m_render_thread;
		

	};
}