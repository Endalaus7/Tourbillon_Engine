#pragma once 

#include "render_system.h"

//负责渲染资源的管理，属于rendersystem的一部分
//实现转移到vulkanrhi
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