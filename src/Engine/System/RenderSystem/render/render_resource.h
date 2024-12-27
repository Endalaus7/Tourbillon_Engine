#pragma once 

#include "render_system.h"

//������Ⱦ��Դ�Ĺ�������rendersystem��һ����

namespace TourBillon
{
	class RenderSource
	{
		friend RenderSystem;
	private:
		void init(RenderSystem* render_system);
	
		void loadTexture(const CEvent&);

		RenderSystem* m_render_system;
	};
}