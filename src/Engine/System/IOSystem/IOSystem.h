#pragma once

#include "System.h"
#include "IOTool.h"


//IOSystem是整个ecs系统对core的接口

namespace TourBillon
{
	struct IOSystemInitInfo :public SystemInitInfo
	{
	};
	class IOSystem :public System
	{
	public:
		virtual void initialize(SystemInitInfo* init_info)override;
	private:

		std::vector<IOTool*> m_IOTools;
	};
}