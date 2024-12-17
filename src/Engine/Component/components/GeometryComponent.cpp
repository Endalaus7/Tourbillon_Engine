#include "GeometryComponent.h"
#include "Math/trans.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

const TBMath::Mat44& TourBillon::Transfrom::GetModelMatrix()
{
	TBMath::Mat44 model_mat = TBMath::modelMatrix(translation, rotation, scale);

	//glm::mat4x4 model = glm::rotate(glm::mat4(1.0f), 0.f, glm::vec3(0.0f, 0.0f, 1.0f));

	return model_mat;
}