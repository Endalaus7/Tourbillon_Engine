#pragma once

#include "common.hpp"
#include "Reflect/Reflect.hpp"
#include "Math/vec2.hpp"
#include "Math/vec3.hpp"
#include "Math/mat44.hpp"

//该文件下的结构体，可能会批量处理，隐藏不要在结构体中添加冗余变量/虚函数

namespace TourBillon
{
	struct Point3d
	{
		Point3d(Real x, Real y, Real z) :pos(x, y, z) {}
		STATIC_PROPERTY_CLASS_BEGIN()
			STATIC_PROPERTY_DEF(TBMath::Vec3, pos)
		STATIC_PROPERTY_CLASS_END()
	};
	struct Point2d
	{
		Point2d(Real x, Real y) :pos(x, y) {}
		STATIC_PROPERTY_CLASS_BEGIN()
			STATIC_PROPERTY_DEF(TBMath::Vec2, pos)
		STATIC_PROPERTY_CLASS_END()
	};

	struct Index_3P
	{
		Index_3P(TBIndex p1, TBIndex p2, TBIndex p3) :
			id1(p1), id2(p2), id3(p3) {}
		STATIC_PROPERTY_CLASS_BEGIN()
			STATIC_PROPERTY_DEF(TBIndex, id1)
			STATIC_PROPERTY_DEF(TBIndex, id2)
			STATIC_PROPERTY_DEF(TBIndex, id3)
		STATIC_PROPERTY_CLASS_END()
	};

}