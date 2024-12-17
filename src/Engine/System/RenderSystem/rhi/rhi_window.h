#pragma once

#include <memory>

namespace TourBillon
{
	struct WindowCreateInfo {
		int         width{ 1280 };
		int         height{ 720 };
		const char* title{ "RenderWindow" };
		bool        is_fullscreen{ false };
	};
	class RHIWindow
	{
	public:
		virtual void initialize(WindowCreateInfo& initialize_info) = 0;
		virtual const WindowCreateInfo& getWindowInfo() = 0;
		virtual void pollEvents() = 0;
		virtual bool shouldClose() = 0;
		
		WindowCreateInfo m_windowInfo;
	};

}