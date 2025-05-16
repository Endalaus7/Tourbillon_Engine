#pragma once

#include <memory>
#include "common.hpp"
//运行rhi脚本的对外接口
//此模块负责处理yaml信息，读取模块在fileIO

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