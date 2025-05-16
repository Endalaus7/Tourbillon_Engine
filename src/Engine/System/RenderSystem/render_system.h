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
		//����ɾ����rendersystemֻ������Ⱦ�ʹ��ڵ�ӳ���ϵ
		//������Ϣ��¼��renderwindow�У���Ӧһ��rhiwindow��
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

		//std::shared_ptr<RHI> m_rhi;//ȫ��Ψһ
		std::shared_ptr<RenderPipelineBase> m_renderPipeline;//��shared_ptr�����rhiwindow��һ��pipelineֻ����һ��shader

		std::shared_ptr<RenderSourceManager> m_rendersourcemanager;//��д��Ⱦ��Դ�ļ�
		

		//RHIBufferResource* m_vertexbuffer;//��δʹ�ã�����Ŀǰ�洢��geometry
		TBVector<Vertex> m_vertex_cache;
	private:
		//TBVector<RHIWindow*> m_rhiWindows;//ע���rhi����ָ��
		
	};
}