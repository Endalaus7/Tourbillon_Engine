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
	public:
		void InsertData(Entity entity, T& component)
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

			if constexpr (std::is_base_of<Assets, T>::value)
			{
				static_cast<decltype(component)>(component).insertData();
			}

			++mSize;
		}

		void RemoveData(Entity entity)
		{
			if (mEntityToIndexMap.find(entity) == mEntityToIndexMap.end())
			{
				LOG_WARNING(std::to_string(entity) + " not exist");
				return;
			}

			
			size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
			size_t indexOfLastElement = mSize - 1;
			if constexpr (std::is_base_of<Assets, decltype(mComponentArray[indexOfRemovedEntity])>::value)
			{
				static_cast<decltype(mComponentArray[indexOfRemovedEntity])>(mComponentArray[indexOfRemovedEntity]).releaseData();
			}
			mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

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
	};
}

