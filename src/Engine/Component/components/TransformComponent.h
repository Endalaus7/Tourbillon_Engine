#pragma once

#include "Component.h"
#include "Math/aabbBox.hpp"

#include "Assets.h"
#include "ColorComponent.h"
#include "ComponentData.h"

//

namespace TourBillon
{
	//void test();

	class RHIBufferResource;

	struct Transfrom :public Component
	{
		STATIC_PROPERTY_CLASS_BEGIN(Transfrom)
			STATIC_PROPERTY_DEF(TBMath::Vec3, position)
			STATIC_PROPERTY_DEF(TBMath::Vec3, rotation)//pitch yaw roll(angle)
			STATIC_PROPERTY_DEF(TBMath::Vec3, scale)
		STATIC_PROPERTY_CLASS_END()
	public:
		const TBMath::Mat44& GetModelMatrix();
		TBMath::Mat44& GetViewMatrix();
		const TBMath::Mat44& GetRotateMatrix();
		
		TBMath::Vec3& getforward();
		TBMath::Vec3& getright();
		TBMath::Vec3& getup();

		void move(const TBMath::Vec3& offset);
		void rotate(const TBMath::Vec3& offset);
	};

}