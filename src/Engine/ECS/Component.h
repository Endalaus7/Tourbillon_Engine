#pragma once
#include "ECSCommon.h"
#include "Reflect/staticReflect.hpp"

//���ʵ��һ�׷���ϵͳ
//������̳�ʱ���޷�ֱ�ӻ�ȡ����ķ������

namespace TourBillon
{
	//class IComponent //�ṩ�����Ľӿڣ���������в���
	//{
	//	virtual void tick(float dt) = 0;
	//};

	
	class Component// :public IComponent	//���ʵ��
	{
		virtual void tick(float dt) {}
	protected:
	};
}

