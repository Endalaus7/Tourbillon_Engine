#pragma once

#include "Component.h"
#include "GeometryComponent.h"

#include "ComponentData.h"
#include <memory>

//�����뼸����ص����

namespace TourBillon
{
	class Renderable:public Component {
	public:
		STATIC_PROPERTY_CLASS_BEGIN(Renderable)
			STATIC_PROPERTY_DEF(std::shared_ptr<Geometry>, mesh)//֮�󻻳��Զ�������ָ�룬�ӳ�ɾ��
		STATIC_PROPERTY_CLASS_END()
	};

	//struct TestComponent : public Transfrom
	//{
	//	//STATIC_PROPERTY_CLASS_BEGIN_PARENT(TestComponent, Transfrom)
	//	STATIC_PROPERTY_CLASS_BEGIN(TestComponent)
	//		STATIC_PROPERTY_DEF(double, x)
	//		STATIC_PROPERTY_DEF(double, y)
	//		STATIC_PROPERTY_CLASS_END()
	//};
}