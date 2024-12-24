#pragma once

#include "Component.h"
#include "Math/vec2.hpp"
#include "Math/vec3.hpp"
#include "Math/mat33.hpp"
#include "Math/mat44.hpp"
#include "Math/aabbBox.hpp"
#include "Math/quaternion.hpp"
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
		void move(const TBMath::Vec3& offset);
		void rotation(const Real& pitch, const Real& yaw, const Real& roll);

		TBMath::Vec3 getDirection();

		TBMath::quaternion GetQuaternion();
		TBMath::Mat33 GetRotationMatrix();
		TBMath::Mat44 GetVPMatrix();
	};
}