#pragma once 

#include "render_system.h"

//������Ⱦ��Դ�Ĺ�������rendersystem��һ����
//ʵ��ת�Ƶ�vulkanrhi
namespace TourBillon
{
	class RenderSourceManager

	{
		//friend RenderSystem;
	public:
		void init(RenderSystem* render_system);
	
		void loadTexture(const CEvent&);
		void releaseTexture(const CEvent&);

		RenderSystem* m_render_system;
	};
}