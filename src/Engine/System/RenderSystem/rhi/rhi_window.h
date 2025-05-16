#pragma once

#include <memory>

#include "ECSManager.h"
#include "components/KeysComponent.h"
#include "components/WindowComponent.h"

//rhiwindow��Ҫָ��renderPort����Ϊһ����ͼ�����ڷ���camera��scene����������Դ
//renderport
namespace TourBillon
{
	class RHIWindow;

	

	struct RHIWindowInfo//����
	{
		//uint32_t         index{ 0 };
		uint32_t         width{ 1280 };
		uint32_t         height{ 720 };
		const char* title{ "RenderWindow" };
		bool        is_fullscreen{ false };

		Entity window_entity;
		
	};
	//rhi���window
	class RHIWindow
	{
	public:
		~RHIWindow();
		virtual void initialize(RHIWindowInfo& initialize_info);
		virtual const RHIWindowInfo& getWindowInfo() = 0;
		virtual void pollEvents() = 0;
		virtual bool shouldClose() = 0;
		virtual void update(float dt) = 0;
		
		//void setCamera(Entity camera_entity);

		FORCE_INLINE uint32_t getwidth() { return m_width; }
		//FORCE_INLINE uint32_t index() { return m_index; }
		FORCE_INLINE uint32_t getheight() { return m_height; }

		RHIWindowInfo m_windowInfo;
	protected:

		Entity m_window_entity;

		//uint32_t m_index = 0;
		uint32_t m_width = 0;//����
		uint32_t m_height = 0;
	};

}