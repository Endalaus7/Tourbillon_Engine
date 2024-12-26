#pragma once

#include "IOTool.h"


namespace TourBillon
{
	class imageIO:public IOTool
	{
	public:
		virtual void init()override;

		void loadPicture(const CEvent&);
	};
}