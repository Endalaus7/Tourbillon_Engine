#pragma once

#include "event.hpp"

//IO工具的运行不需要实体

namespace TourBillon
{
	class IOTool
	{
		virtual void init() = 0;
	};
}