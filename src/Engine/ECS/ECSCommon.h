#pragma once
#include "common.hpp"
#include "log.hpp"
#include <bitset>
//管理system的类，外界使用system的接口
//提供时间管理

namespace TourBillon
{
	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;//ComponentType对应于签名中的一个位

	using Entity = std::uint32_t;
	const Entity MAX_ENTITIES = 5000;

	
}

