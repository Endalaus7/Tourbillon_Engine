#pragma once
#include "common.hpp"
#include "log.hpp"
#include <bitset>
//����system���࣬���ʹ��system�Ľӿ�
//�ṩʱ�����

namespace TourBillon
{
	using ComponentType = std::uint8_t;
	const ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;//ComponentType��Ӧ��ǩ���е�һ��λ

	using Entity = std::uint32_t;
	const Entity MAX_ENTITIES = 5000;

	
}

