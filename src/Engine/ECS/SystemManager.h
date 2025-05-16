#pragma once
#include "ECSCommon.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include "System.h"
#include <typeindex>
#include <typeinfo>
#include <bitset>
#include <type_traits>

//管理system的类，外界使用system的接口
//提供时间管理

namespace TourBillon
{
	class SystemManager
	{
	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			std::string typeName = typeid(T).name();//std::type_index(typeid(T));

			if (!std::is_base_of<System, T>::value)
			{
				LOG_WARNING(typeName + " is Not system class");
				return nullptr;
			}


			auto find_sys_itr = m_systems.find(typeName);
			if (find_sys_itr != m_systems.end())
				return std::static_pointer_cast<T>(m_systems[typeName]);

			std::shared_ptr<T> newSystem = std::make_shared<T>();
			m_systems[typeName] = newSystem;
			return newSystem;
		}
		template<typename T>
		std::shared_ptr<T> GetSystem()
		{
			std::string typeName = typeid(T).name();//std::type_index(typeid(T));

			auto find_sys_itr = m_systems.find(typeName);
			if (find_sys_itr != m_systems.end())
				return *find_sys_itr;

			return nullptr;
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			std::type_index typeIndex = std::type_index(typeid(T));

			if(mSignatures.find(typeIndex) != mSignatures.end())
			{
				LOG_WARNING("System used before registered.");
				return;
			}

			mSignatures.insert({ typeIndex, signature });
		}

		void EntityDestroyed(Entity entity)
		{
			for (auto const& pair : m_systems)
			{
				auto const& system = pair.second;


				system->mEntities.erase(entity);
			}
		}
		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			for (auto const& pair : m_systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = mSignatures[type];

				if ((entitySignature & systemSignature) == systemSignature)
				{
					system->mEntities.insert(entity);
				}
				else
				{
					system->mEntities.erase(entity);
				}
			}
		}
	protected:
		// 包含所有的签名
		std::unordered_map<std::string, Signature> mSignatures{};

		// 包含所有的System
		std::unordered_map<std::string, std::shared_ptr<System>> m_systems{};
	};
}

