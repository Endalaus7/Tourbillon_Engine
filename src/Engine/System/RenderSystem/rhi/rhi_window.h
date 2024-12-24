#pragma once

#include <memory>

#include "ECSManager.h"
#include "components/KeysComponent.h"
#include "components/WindowComponent.h"

namespace TourBillon
{
	class RHIWindow;
	struct WindowCreateInfo {
		uint32_t         index{ 0 };
		uint32_t         width{ 1280 };
		uint32_t         height{ 720 };
		const char* title{ "RenderWindow" };
		bool        is_fullscreen{ false };
		std::shared_ptr<RHIWindow> sharedwindow{ nullptr };
	};
	class RHIWindow
	{
	public:
		~RHIWindow();
		virtual void initialize(WindowCreateInfo& initialize_info);
		virtual const WindowCreateInfo& getWindowInfo() = 0;
		virtual void pollEvents() = 0;
		virtual bool shouldClose() = 0;
		
		void setCamera(Entity camera_entity);

		FORCE_INLINE uint32_t getwidth() { return m_width; }
		FORCE_INLINE uint32_t index() { return m_index; }
		FORCE_INLINE uint32_t getheight() { return m_height; }

		WindowCreateInfo m_windowInfo;
	protected:
		static Entity m_window_entity;

		uint32_t m_index = 0;
		uint32_t m_width = 0;
		uint32_t m_height = 0;
	};

}