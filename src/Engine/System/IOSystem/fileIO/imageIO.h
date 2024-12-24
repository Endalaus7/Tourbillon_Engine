#pragma once

#include "IOTool.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace TourBillon
{
	class imageIO:public IOTool
	{
	public:
		virtual void init()override;
	};
}