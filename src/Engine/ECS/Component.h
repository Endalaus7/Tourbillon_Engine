#pragma once
#include "ECSCommon.h"
#include "Reflectable.h"

//���ʵ��һ�׷���ϵͳ
//������̳�ʱ���޷�ֱ�ӻ�ȡ����ķ������

//add:component��Ҫ����һ��Reactable����

namespace TourBillon
{
	class Component : public Reflectable
	{
	public:
		virtual void tick(float dt) {}
		virtual void insertData(){}
		virtual void releaseData(){}
	protected:
		//virtual void 
	};

	

}

