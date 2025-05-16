#pragma once
#include "ECSCommon.h"
#include "Reflectable.h"

//组件实现一套反射系统
//组件被继承时，无法直接获取父类的反射变量

//add:component需要再套一层Reactable父类

namespace TourBillon
{
	class Component : public Reflectable
	{
	public:
		virtual void tick(float dt) {}
		virtual void insertData(){}
		virtual void releaseData(){}
	protected:
		//virtual void 
	};

	

}

