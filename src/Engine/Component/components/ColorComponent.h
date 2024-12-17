#pragma once

#include "Component.h"
#include "Math/vec2.hpp"
#include "Math/vec3.hpp"
#include "Math/vec4.hpp"

//�����뼸����ص����

namespace TourBillon
{
	struct ColorRGBA
	{
		ColorRGBA(Real x, Real y, Real z, Real w) :rgba(x, y, z, w) {}
		STATIC_PROPERTY_CLASS_BEGIN(ColorRGBA)
			STATIC_PROPERTY_DEF(TBMath::Vec4, rgba)
			STATIC_PROPERTY_CLASS_END()
	};
}