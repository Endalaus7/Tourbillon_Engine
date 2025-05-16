#pragma once

#include "Component.h"
#include "Math/vec2.hpp"



namespace TourBillon
{

	//视口组件,绑定在渲染实体上
	//可能不需要该组件
	struct RenderPort : public Component
	{
		RenderPort() {}
		STATIC_PROPERTY_CLASS_BEGIN()
			STATIC_PROPERTY_DEF(Entity, camera)
		STATIC_PROPERTY_CLASS_END()


	public:
		void setParent(Entity parent_entity);
	protected:
		uint32_t indexedNum = 0;//被索引次数
		bool isPooled = false;  // 是否在对象池中待复用
	};

	struct RenderPipeline : public Component
	{
		
	};
}