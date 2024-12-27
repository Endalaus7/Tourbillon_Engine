#pragma once
#include "ECSCommon.h"
#include "Reflect/staticReflect.hpp"

//组件实现一套反射系统
//组件被继承时，无法直接获取父类的反射变量

namespace TourBillon
{
	//class IComponent //提供给外界的接口，对组件进行操作
	//{
	//	virtual void tick(float dt) = 0;
	//};

	
	class Component// :public IComponent	//组件实体
	{
	public:
		virtual void tick(float dt) {}
		virtual void insertData() {}
		virtual void releaseData() {}
	protected:
	};
}

