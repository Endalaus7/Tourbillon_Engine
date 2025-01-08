#pragma once

#include "Component.h"
#include "Math/vec2.hpp"

//所有与几何相关的组件

namespace TourBillon
{
	struct Buttons : public Component
	{
		STATIC_PROPERTY_CLASS_BEGIN()
			STATIC_PROPERTY_DEF(uint32_t, baseWindow)
			STATIC_PROPERTY_DEF(KeyValues, keyvalue)
		STATIC_PROPERTY_CLASS_END()
	};

	struct Mouse : public Component
	{
		Mouse() :pressLeft(false), pressRight(false) {}
		STATIC_PROPERTY_CLASS_BEGIN()
			STATIC_PROPERTY_DEF(uint32_t, baseWindow)
			STATIC_PROPERTY_DEF(bool, pressLeft)
			STATIC_PROPERTY_DEF(bool, pressRight)
			STATIC_PROPERTY_DEF(TBMath::Vec2, mousePos)//offset base on window
			STATIC_PROPERTY_DEF(TBMath::Vec2, mouseOffset)//offset base on window
		STATIC_PROPERTY_CLASS_END()
	};
}