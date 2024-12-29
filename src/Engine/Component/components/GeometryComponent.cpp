#include "GeometryComponent.h"
#include "Math/trans.hpp"
#include "ECSManager.h"

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>


TourBillon::Geometry* TourBillon::GeometryShared::loadData()
{
	//发送加载事件，IO系统处理
	ECSManager::Instance()->SendEvent(Events::LOAD_MODEL, (void*)this);
	return dynamic_cast<Geometry*>(assetdata);
}
#if 0
void TourBillon::test()
{
	//if (IsReflected_t<TestComponent>)
	//{
	//	TestComponent testcom;
	//
	//	forEachProperty_rec(testcom, [&](auto&& fieldName, auto&& value, size_t depth)
	//		{
	//			int a = 0;
	//		});
	//}
	if (IsReflected_t<Geometry>)
	{
		Geometry geometry;

		//auto& x = Geometry::FIELD<Geometry, 0>(std::forward<Geometry>(geometry)).value();
		auto& x = get_property_value<0>(geometry);
		//x = 0;
		//auto namee0 = Geometry::FIELD<Geometry, 0>::name();
		auto namee0 = get_property_name<0>(geometry);
		auto namee1 = get_property_name<0, Geometry>();
		//int a = 0;
		forEachProperty_rec(geometry, [&](auto&& fieldName, auto&& value, size_t depth)
			{
				if (IsInt_v(value))
				{
					int b = 0;
				}
			});
		int a = 0;

	}
}
#endif