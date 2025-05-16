#pragma once

#include "Component.h"
#include "Math/vec2.hpp"



namespace TourBillon
{

	//�ӿ����,������Ⱦʵ����
	//���ܲ���Ҫ�����
	struct RenderPort : public Component
	{
		RenderPort() {}
		STATIC_PROPERTY_CLASS_BEGIN()
			STATIC_PROPERTY_DEF(Entity, camera)
		STATIC_PROPERTY_CLASS_END()


	public:
		void setParent(Entity parent_entity);
	protected:
		uint32_t indexedNum = 0;//����������
		bool isPooled = false;  // �Ƿ��ڶ�����д�����
	};

	struct RenderPipeline : public Component
	{
		
	};
}