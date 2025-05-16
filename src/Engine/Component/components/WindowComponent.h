#pragma once

#include "Component.h"
#include "Math/vec2.hpp"

//窗口组件

namespace TourBillon
{
	//相对位置和尺寸
	struct WindowRect
	{
		float left = 0.f;
		float top = 0.f;
		float width = 1.f;
		float height = 1.f;
	};

	struct ECSRenderPort
	{
		uint32_t index = -1;//在当前window下的index
		bool isActive = true;
		Entity renderport;
		WindowRect bounds;
	};

	struct ECSWindow : public Component
	{
		ECSWindow() 
			:title("Tourbillon Engine"),
			isFullscreen(false)
		{}
		STATIC_PROPERTY_CLASS_BEGIN()
			//STATIC_PROPERTY_DEF(uint32_t, index)
			//STATIC_PROPERTY_DEF(Entity, camera)
			STATIC_PROPERTY_DEF(uint32_t, width)
			STATIC_PROPERTY_DEF(uint32_t, height)
			STATIC_PROPERTY_DEF(std::string, title)
			STATIC_PROPERTY_DEF(bool, isFullscreen)
			//STATIC_PROPERTY_DEF(TBVector<ECSRenderPort*>, renderports)

		STATIC_PROPERTY_CLASS_END()

	public:
		
		void closePort(Entity renderport);
		void addChild(Entity renderport);
	protected:
		TBVector<ECSRenderPort> renderports;
	};


}