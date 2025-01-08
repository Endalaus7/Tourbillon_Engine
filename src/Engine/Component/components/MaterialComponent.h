#pragma once

#include "Component.h"
#include "ComponentData.h"

#include "ColorComponent.h"
#include "ShaderComponent.h"

#include "Assets.h"

//所有与几何相关的组件

namespace TourBillon
{
	struct Material :public Component
	{
		STATIC_PROPERTY_CLASS_BEGIN()
			STATIC_PROPERTY_DEF(ColorRGBA, basecolor)
			STATIC_PROPERTY_DEF(TexturePtr, MainTexture)
			//STATIC_PROPERTY_DEF(PassPtr, passes)
			//...
			STATIC_PROPERTY_CLASS_END()
	public:

	};
}