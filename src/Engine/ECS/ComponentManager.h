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
		template<typename T>
		void RegisterComponent()
		{
			if (!std::is_base_of<Component, T>::value)
			{
				LOG_WARNING("Register is Not component class");
				return;
			}

			std::type_index typeIndex = std::type_index(typeid(T));
			if(m_ComponentArrays.find(typeIndex) != m_ComponentArrays.end())
			{
				LOG_WARNING("this has been Register");
				return;
			}
			std::shared_ptr<ComponentArray<T>> new_array = std::make_shared<ComponentArray<T>>();
			new_array->componentType = mNextComponentType;
			m_ComponentArrays.insert({ typeIndex, new_array });

			++mNextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			std::type_index typeIndex = std::type_index(typeid(T));

			if (m_ComponentArrays.find(typeIndex) == m_ComponentArrays.end())
			{
				LOG_WARNING("Component not registered before use.");
				return 0;
			}

			return m_ComponentArrays[typeIndex]->componentType;
		}

		template<typename T>
		 TBVector<Entity> GetComponentEntities()
		{
			std::type_index typeIndex = std::type_index(typeid(T));

			if (m_ComponentArrays.find(typeIndex) == m_ComponentArrays.end())
			{
				LOG_ERROR("Component not registered before use.");
			}

			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeIndex])->GetEntities();
		}
	private:
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			std::type_index typeIndex = std::type_index(typeid(T));

			if(m_ComponentArrays.find(typeIndex) == m_ComponentArrays.end())
			{
				LOG_WARNING("has not register as a component");
				return nullptr;
			}

			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeIndex]);
		}
	public:
		template<typename T>
		size_t GetComponentArraySize()
		{
			return GetComponentArray<T>()->GetSize();
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
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
		std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_ComponentArrays;
		ComponentType mNextComponentType = 0;
	};
}

