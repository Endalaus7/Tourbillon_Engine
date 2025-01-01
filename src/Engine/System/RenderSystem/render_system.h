#pragma once
#include <memory>
#include <queue>
#include "System.h"
#include "rhi/rhi.h"
#include "../Component/components/GeometryComponent.h"

#include "rhi/rhi_source.h"

namespace TourBillon
{
	struct WindowCreateInfo;
	class RenderPipeline;
	class RenderSource;

	struct RenderSystemInitInfo:public SystemInitInfo
	{
		RHI_TYPE rhi_type;
		uint32_t frame_rate = 60;
		uint32_t window_width = 500;
		uint32_t window_height = 500;
		uint32_t window_num = 1;
		//std::shared_ptr<RenderWindow> window_system;
		//std::shared_ptr<DebugDrawManager> debugdraw_manager;
	};
	
	class RenderSystem :public System
	{
	public:
		~RenderSystem();
		virtual void initialize(SystemInitInfo* init_info) override;

		void rendLoop(std::function<void(float)> beforeRender, std::function<void(float)> afterRender);
		void clear();

		void loadMeshBuffer(Geometry& mesh);

		void SetMainCamera(uint32_t windowindex, Entity camera);

		void clearBuffers();

		bool shouldClose();

		uint32_t m_frame_rate = 30;

		std::shared_ptr<RHI> m_rhi;
		TBVector<std::shared_ptr<RHIWindow>> m_rhiWindows;
		std::shared_ptr<RenderPipeline> m_renderPipeline;
		std::shared_ptr<RenderSource> m_rendersource;
		

		RHIBufferResource* m_vertexbuffer;//暂未使用，顶点存储在geometry
		std::vector<Vertex>m_vertex_cache;

		
	};
}