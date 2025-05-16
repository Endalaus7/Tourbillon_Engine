#pragma once
#include <memory>
#include <queue>
#include "System.h"
#include "rhi/rhi.h"
#include "../Component/components/GeometryComponent.h"

#include "rhi/rhi_source.h"

namespace TourBillon
{
	struct RHIWindowInfo;
	class RenderPipelineBase;
	class RenderSourceManager;

	struct RenderSystemInitInfo:public SystemInitInfo
	{
		RHI_TYPE rhi_type;
		uint32_t frame_rate = 60;
		//后续删除，rendersystem只分配渲染和窗口的映射关系
		//窗口信息记录在renderwindow中（对应一个rhiwindow）
		//uint32_t window_width = 500;
		//uint32_t window_height = 500;
		//uint32_t window_num = 1;
		//std::shared_ptr<RenderWindow> window_system;
		//std::shared_ptr<DebugDrawManager> debugdraw_manager;
	};
	
	class RenderSystem :public System
	{
	public:
		~RenderSystem();
		virtual void initialize(SystemInitInfo* init_info) override;

		//void registerRHIWindow(RHIWindow* rhiwindow);

		void rendLoop(std::function<void(float)> beforeRender, std::function<void(float)> afterRender);
		void clear();

		void loadMeshBuffer(GeometryData& mesh);

		//void SetMainCamera(uint32_t windowindex, Entity camera);

		void clearBuffers();

		bool shouldClose();

		float m_frame_rate = 30.f;

		//std::shared_ptr<RHI> m_rhi;//全局唯一
		std::shared_ptr<RenderPipelineBase> m_renderPipeline;//用shared_ptr分配给rhiwindow，一个pipeline只能用一套shader

		std::shared_ptr<RenderSourceManager> m_rendersourcemanager;//读写渲染资源文件
		

		//RHIBufferResource* m_vertexbuffer;//暂未使用，顶点目前存储在geometry
		TBVector<Vertex> m_vertex_cache;
	private:
		//TBVector<RHIWindow*> m_rhiWindows;//注册的rhi窗口指针
		
	};
}