#pragma once

#include <memory>
#include "common.hpp"
//����rhi�ű��Ķ���ӿ�
//��ģ�鸺����yaml��Ϣ����ȡģ����fileIO

namespace TourBillon
{
	struct RHIScriptInfo
	{

	};
	class RHIScript
	{
		virtual void initialize(RHIScriptInfo& initialize_info) = 0;
	};
	
}