#pragma once
#include "ECSCommon.h"
#include "Reflect/staticReflect.hpp"

//���ʵ��һ�׷���ϵͳ
//������̳�ʱ���޷�ֱ�ӻ�ȡ����ķ������

namespace TourBillon
{
	class Component
	{
	public:
		virtual void tick(float dt) {}
		virtual void insertData(){}
		virtual void releaseData(){}
	protected:
	};

	

}

