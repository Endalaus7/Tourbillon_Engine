#pragma once

#include "common.hpp"
#include "Reflect/staticReflect.hpp"
#include "Math/vec2.hpp"
#include "Math/vec3.hpp"
#include "Math/mat44.hpp"

//���ļ��µĽṹ�壬���ܻ������������ز�Ҫ�ڽṹ��������������/�麯��

namespace TourBillon
{
	struct Point3d
	{
		Point3d(Real x, Real y, Real z) :pos(x, y, z) {}
		STATIC_PROPERTY_CLASS_BEGIN(Point3d)
			STATIC_PROPERTY_DEF(TBMath::Vec3, pos)
		STATIC_PROPERTY_CLASS_END()
	};
	struct Point2d
	{
		Point2d(Real x, Real y) :pos(x, y) {}
		STATIC_PROPERTY_CLASS_BEGIN(Point2d)
			STATIC_PROPERTY_DEF(TBMath::Vec2, pos)
		STATIC_PROPERTY_CLASS_END()
	};

	struct Index_3P
	{
		Index_3P(TBIndex p1, TBIndex p2, TBIndex p3) :
			id1(p1), id2(p2), id3(p3) {}
		STATIC_PROPERTY_CLASS_BEGIN(Index_3P)
			STATIC_PROPERTY_DEF(TBIndex, id1)
			STATIC_PROPERTY_DEF(TBIndex, id2)
			STATIC_PROPERTY_DEF(TBIndex, id3)
		STATIC_PROPERTY_CLASS_END()
	};

}