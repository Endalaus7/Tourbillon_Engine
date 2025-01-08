#pragma once

#include "Component.h"
#include "ComponentData.h"
#include "TextureComponent.h"
#include "ColorComponent.h"

#include "Assets.h"

//所有与几何相关的组件

namespace TourBillon
{
	
	struct Pass
	{
		STATIC_PROPERTY_CLASS_BEGIN()
			
		STATIC_PROPERTY_CLASS_END()
	};

	
	struct ShaderData
	{
		STATIC_PROPERTY_CLASS_BEGIN()
			STATIC_PROPERTY_DEF()
		STATIC_PROPERTY_CLASS_END()
	};
	//并非glsl shader，而是代表整个pass的抽象集合体
	struct ShaderPtr : public Assets<ShaderData>
	{
		~ShaderPtr()override {}
		virtual ShaderData* loadData()override;
		virtual void releaseData()override;
	};
}