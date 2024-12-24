#pragma once

#include "IOTool.h"
#include "event.hpp"

#include "Math/vec3.hpp"

namespace TourBillon
{
	class keyIO:public IOTool
	{
	public:
		virtual void init()override;
	protected:
		void updateKeyState(const CEvent&);
		void updateMouseState(const CEvent&);
		
	};
}