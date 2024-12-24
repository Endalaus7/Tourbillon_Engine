#pragma once

#include "Component.h"
#include "Math/vec2.hpp"

//所有与几何相关的组件

namespace TourBillon
{
	struct RenderWindow : public Component
	{
		RenderWindow() 
			:title("render window "+std::to_string(index)),
			isFullscreen(false)
		{}
		STATIC_PROPERTY_CLASS_BEGIN(RenderWindow)
			STATIC_PROPERTY_DEF(uint32_t, index)
			STATIC_PROPERTY_DEF(Entity, camera)
			STATIC_PROPERTY_DEF(uint32_t, width)
			STATIC_PROPERTY_DEF(uint32_t, height)
			STATIC_PROPERTY_DEF(std::string, title)
			STATIC_PROPERTY_DEF(bool, isFullscreen)

		STATIC_PROPERTY_CLASS_END()
	};
}