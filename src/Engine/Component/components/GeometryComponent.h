#pragma once

#include "Component.h"
#include "Math/aabbBox.hpp"

#include "Assets.h"
#include "ColorComponent.h"
#include "ComponentData.h"
#include "TransformComponent.h"

//所有与几何相关的组件

namespace TourBillon
{
	//void test();

	class RHIBufferResource;

	struct Vertex
	{
		Vertex( Point3d pos_arg, Point2d uv_arg, ColorRGBA color_arg) :
			pos(pos_arg), uv(uv_arg), color(color_arg) {}
		STATIC_PROPERTY_CLASS_BEGIN(Vertex)
			STATIC_PROPERTY_DEF(Point3d, pos)
			STATIC_PROPERTY_DEF(Point2d, uv)
			//Normal
			//Tangent
			STATIC_PROPERTY_DEF(ColorRGBA, color)//delete after
			//...
		STATIC_PROPERTY_CLASS_END()
	};

	//static mesh
	struct Geometry :public AssetsData
	{
		STATIC_PROPERTY_CLASS_BEGIN(Geometry)
			STATIC_PROPERTY_DEF(ReflectPath, modelPath)//可以是相对路径
			//STATIC_PROPERTY_DEF(AABBBox, geometryAABB)//TODO
			STATIC_PROPERTY_DEF_ARRAY(Vertex, vertexArray)
			STATIC_PROPERTY_DEF_ARRAY(Index_3P, indexArray)
		STATIC_PROPERTY_CLASS_END()

	public:
		RHIBufferResource* vertexBuffer = nullptr;
		RHIBufferResource* indexBuffer = nullptr;
	};

	struct GeometryShared :public Assets
	{
		~GeometryShared()override {}
		virtual Geometry* loadData()override;
	}; 

	//struct TestComponent : public Transfrom
	//{
	//	//STATIC_PROPERTY_CLASS_BEGIN_PARENT(TestComponent, Transfrom)
	//	STATIC_PROPERTY_CLASS_BEGIN(TestComponent)
	//		STATIC_PROPERTY_DEF(double, x)
	//		STATIC_PROPERTY_DEF(double, y)
	//		STATIC_PROPERTY_CLASS_END()
	//};
}