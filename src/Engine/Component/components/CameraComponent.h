#pragma once

#include "Component.h"
#include "Math/vec2.hpp"
#include "Math/vec3.hpp"
#include "Math/mat44.hpp"
#include "Math/aabbBox.hpp"
#include "ComponentData.h"

//所有与几何相关的组件

namespace TourBillon
{
	struct Camera3D:public Component
	{
		STATIC_PROPERTY_CLASS_BEGIN(Camera3D)
			STATIC_PROPERTY_DEF(TBMath::Vec3, pos)
			STATIC_PROPERTY_DEF(TBMath::Vec3, lookat)
			STATIC_PROPERTY_DEF(TBMath::Vec3, up)
			STATIC_PROPERTY_DEF(bool, isOrthographic)
			STATIC_PROPERTY_DEF(float, fovX)//degrees
			STATIC_PROPERTY_DEF(float, fovY)//degrees
			STATIC_PROPERTY_DEF(float, nearClip)
			STATIC_PROPERTY_DEF(float, farClip)
		STATIC_PROPERTY_CLASS_END()
	public:
		TBMath::Mat44 GetVPMatrix();
	};
}