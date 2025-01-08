#pragma once
#include "ECSCommon.h"
#include "Reflect/Reflect.hpp"

//组件实现一套反射系统
//组件被继承时，无法直接获取父类的反射变量

namespace TourBillon
{
	class Component
	{
	public:
		virtual void tick(float dt) {}
		virtual void insertData(){}
		virtual void releaseData(){}
	protected:
	};

	

}

