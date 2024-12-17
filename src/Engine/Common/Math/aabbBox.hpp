#pragma once
#include "common.hpp"
#include "vec3.hpp"

namespace TBMath
{
	class AABBBox
	{
	public:
		AABBBox()
			: aabbmin(), aabbmax()
		{}

		AABBBox(Vec3 argmin, Vec3 argmax)
			: aabbmin(argmin), aabbmax(argmax)
		{}

		

		Vec3 aabbmin, aabbmax;
	};
}
