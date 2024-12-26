#pragma once

#include "System.h"
#include "IOTool.h"




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