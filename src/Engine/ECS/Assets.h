#pragma once
#include "Component.h"

//资源管理在IOSystem

namespace TourBillon
{

	//数据的接口
	//同样支持反射
	class AssetsData
	{

	};
	//仅存储路径
	class Assets : public Component
	{
	public:
		ReflectPath assetpath;//绝对路径,用于标识唯一资产
	};

	
}

