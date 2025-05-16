#pragma once
#include "ECSCommon.h"
#include "Reflect/Reflect.hpp"
namespace TourBillon
{
	class ReflectableBase
	{

	};

	template<typename T>
	class Reflectable :public ReflectableBase
	{
		struct Registrar_Reflect {
			Registrar_Reflect() {
				std::string className = demangle(typeid(T).name());
				auto& registry = Reflectable<T>::getRegistry();
				if (!registry.contains(className)) {
					registry[className] = []() -> T* { return new T(); };
				}
			}
		};

		static std::string getClassName()
		{
			return typeid(T).name();
		}
	};


	//std::map<std::string, std::function<Reflectable* ()>> classRegistry;
	std::unordered_map<std::string, std::function<std::shared_ptr<ReflectableBase> ()>> s_static_reflect_Registry;//æ≤Ã¨∑¥…‰¿‡–Õ◊¢≤·
	void Registerclass(const std::string& className, std::function<std::shared_ptr<Reflectable> ()> creator) 
	{
		s_static_reflect_Registry[className] = creator;
	}

	
}