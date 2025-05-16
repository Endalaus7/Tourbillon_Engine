#pragma once
#include "System.h"

//´°¿ÚÏµÍ³

namespace TourBillon
{
	struct CustomWindowSystemInitInfo :public SystemInitInfo
	{
		
	};

	class CustomWindowSystem :public System
	{
	public:
		~CustomWindowSystem();
		virtual void initialize(SystemInitInfo* init_info) override;
	};
}