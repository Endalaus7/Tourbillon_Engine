#pragma once
#include "ECSCommon.h"
#include "Component.h"
#include "ComponentArray.h"
#include <unordered_map>

#include <typeindex>
#include <assert.h>


namespace TourBillon
{
	class ComponentManager
	{
	public:

		void RegisterComponent(std::string type_name)
		{
			if (m_ComponentArrays.find(type_name) != m_ComponentArrays.end())
			{
				LOG_WARNING("this has been Register");
				return;
			}

			s_static_reflect_Registry;
		}
		template<typename T>
		void RegisterComponent()
		{
			std::string typeName = typeid(T).name();
			if (m_ComponentArrays.find(typeName) != m_ComponentArrays.end())
			{
				LOG_WARNING("this has been Register");
				return;
			}

			if (!std::is_base_of<Component, T>::value)
			{
				LOG_ERROR("Register is Not component class");
				return;
			}
			//std::type_index typeIndex = std::type_index(typeid(T));
			
			std::shared_ptr<ComponentArray<T>> new_array = std::make_shared<ComponentArray<T>>();
			new_array->componentType = mNextComponentType;
			m_ComponentArrays.insert({ typeName, new_array });

			++mNextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			//std::type_index typeIndex = std::type_index(typeid(T));
			std::string typeName = typeid(T).name();
			if (m_ComponentArrays.find(typeName) == m_ComponentArrays.end())
			{
				LOG_WARNING("Component not registered before use.");
				return 0;
			}

			return m_ComponentArrays[typeName]->componentType;
		}

		template<typename T>
		 TBVector<Entity> GetComponentEntities()
		{
			//std::type_index typeIndex = std::type_index(typeid(T));
			std::string typeName = typeid(T).name();
			if (m_ComponentArrays.find(typeName) == m_ComponentArrays.end())
			{
				LOG_ERROR("Component not registered before use.");
			}

			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName])->GetEntities();
		}
	private:
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			std::string typeName = typeid(T).name();
			//std::type_index typeIndex = std::type_index(typeid(T));

			if(m_ComponentArrays.find(typeName) == m_ComponentArrays.end())
			{
				LOG_WARNING("has not register as a component");
				return nullptr;
			}

			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
		}
	public:
		template<typename T>
		size_t GetComponentArraySize()
		{
			return GetComponentArray<T>()->GetSize();
		}

		template<typename T>
		void AddComponent(Entity entity, T& component)
		{
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			GetComponentArray<T>()->RemoveData(entity);
		}
		template<typename T>
		T& GetComponent(Entity entity)
		{
			//没有判断组件未注册
			return GetComponentArray<T>()->GetData(entity);
		}
		void EntityDestroyed(Entity entity)
		{
			for (auto const& pair : m_ComponentArrays)
			{
				auto const& component = pair.second;

				component->EntityDestroyed(entity);
			}
		}
	protected:
		std::unordered_map<std::string, std::shared_ptr<IComponentArray>> m_ComponentArrays;//组件数组
		ComponentType mNextComponentType = 0;
	};
}

