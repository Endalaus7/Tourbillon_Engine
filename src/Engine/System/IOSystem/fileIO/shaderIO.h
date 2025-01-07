#pragma once

#include "IOTool.h"


namespace TourBillon
{
	class shaderIO :public IOTool
	{
	public:
		virtual void init()override;

		void loadShader(const CEvent&);
	};
}