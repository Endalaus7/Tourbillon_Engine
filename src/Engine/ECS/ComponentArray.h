#pragma once
#include "ECSCommon.h"
#include "Component.h"
#include <unordered_map>
#include "Assets.h"
//管理Component的类，实现紧凑排布，不需要if(vaild)检查
//index:实体对应的组件数组索引

namespace TourBillon
{
	class IComponentArray 
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;


		ComponentType componentType;//分配给签名的
	};
	template<typename T>
	class ComponentArray:public IComponentArray
	{
		template<typename U, typename V>
		void insertAssetData(U&& fieldName, V&& value, size_t depth)
		{
			if constexpr (std::is_base_of_v<Assets, std::remove_reference_t<decltype(value)>>)
			{
				((decltype(value))value).insertData();
				forEachProperty_rec(value, METHOD_PROPERTY(ComponentArray<T>::insertAssetData));
			}
		}
		template<typename U, typename V>
		void releaseAssetData(U&& fieldName, V&& value, size_t depth)
		{
			if constexpr (std::is_base_of_v<Assets, std::remove_reference_t<decltype(value)>>)
			{
				forEachProperty_rev(value, METHOD_PROPERTY(ComponentArray<T>::releaseAssetData));
				((decltype(value))value).releaseData();
			}
		}
	public:
		constexpr void InsertData(Entity entity, T& component)
		{
			if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
			{
				LOG_WARNING(std::to_string(entity)+" has been insert");
					return;
			}

			size_t newIndex = mSize;
			mEntityToIndexMap[entity] = newIndex;
			mIndexToEntityMap[newIndex] = entity;
			mComponentArray[newIndex] = component;
			
			component.insertData();
			if constexpr(IsReflected_t<T>)
			{
				forEachProperty_rec(component, [&](auto&& fieldName, auto&& value, size_t depth) {
					if constexpr (std::is_base_of_v<Component, std::remove_reference_t<decltype(value)>>)
					{
						((decltype(value))value).insertData();
					}
					});
			}

			++mSize;
		}

		constexpr void RemoveData(Entity entity)
		{
			if (mEntityToIndexMap.find(entity) == mEntityToIndexMap.end())
			{
				LOG_WARNING(std::to_string(entity) + " not exist");
				return;
			}

			
			size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
			size_t indexOfLastElement = mSize - 1;
			T& component = mComponentArray[indexOfRemovedEntity];
			
			if constexpr (IsReflected_t<T>)
			{
				forEachProperty_rev(component, [&](auto&& fieldName, auto&& value, size_t depth) {
					if constexpr (std::is_base_of_v<Component, std::remove_reference_t<decltype(value)>>)
					{
						((decltype(value))value).releaseData();
					}
					});
			}
			component.releaseData();

			component = mComponentArray[indexOfLastElement];

			// Update map to point to moved spot
			Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
			mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			mEntityToIndexMap.erase(entity);
			mIndexToEntityMap.erase(indexOfLastElement);

			--mSize;
		}

		T& GetData(Entity entity)
		{
			if(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end())
			{
				LOG_ERROR(std::to_string(entity) + " not exist");
			}
			return mComponentArray[mEntityToIndexMap[entity]];
		}
		TBVector<Entity> GetEntities()//获取包含本组件的所有实体
		{
			TBVector<Entity> entites(mSize);
			for (int i = 0; i < mSize; i++)
			{
				entites[i] = mIndexToEntityMap[i];
			}
			return entites;
		}
		void EntityDestroyed(Entity entity) override
		{
			if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
			{
				RemoveData(entity);
			}
		}

		FORCE_INLINE size_t GetSize() { return mSize; }
	protected:
		std::array<T, MAX_ENTITIES> mComponentArray;//data
		std::unordered_map<Entity, size_t> mEntityToIndexMap;
		std::unordered_map<size_t, Entity> mIndexToEntityMap;
		size_t mSize = 0;

	private:
		
	};
}

