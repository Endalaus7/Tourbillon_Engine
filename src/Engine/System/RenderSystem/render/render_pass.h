#pragma once
#include <memory>
#include "rhi/rhi.h"
#include "Structure/TB_List.hpp"

//弃用

namespace TourBillon
{
	struct RenderPassInitInfo{
		//std::shared_ptr<RHI>                rhi;
		//std::shared_ptr<RenderResource> render_resource;
	};
	class RenderPipelineBase;

	//renderpass应当也是组件化的，解决pass和视口之间互相复用的问题
	//多个视口用一个pass时，使用两个subpass，或特殊情况使用VK_KHR_multiview扩展（VR,暂不考虑）


	class RenderPass
	{
	public:
		struct FrameBufferAttachment
		{
			RHIImage*		image;
			RHIDeviceMemory* mem;
			RHIImageView*	view;
			RHIFormat       format;
		};
		struct Framebuffer
		{
			int           width;
			int           height;
			TBVector<TBAlignedArray<RHIFramebuffer*>> framebuffers;//1维窗口，2维交换帧
			RHIRenderPass* render_pass;

			std::vector<FrameBufferAttachment> attachments;
		};

		struct Descriptor
		{
			RHIDescriptorSetLayout* layout;
			RHIDescriptorSet* descriptor_set;
		};

		virtual void initialize(const RenderPassInitInfo* init_info);
		virtual void postInitialize();
		virtual void updateDescriptorSets(float dt, RHIDrawInfo& drawinfo) = 0;

		virtual void drawPass(float dt, RHIDrawInfo& drawinfo) = 0;
		virtual void beforeDraw(float dt, RHIDrawInfo& drawinfo) = 0;

		virtual void setup_DescriptorSetLayout() {}
		virtual void setup_RenderPass() {}
		virtual void setup_Pipeline() {}
		//virtual void setup_FrameBuffer() {}

		virtual void destroyFramebuffer() {}
	protected:
		//std::shared_ptr<RHI>                m_rhi;
		//std::shared_ptr<RenderResource> m_render_resource;

		TBVector<Descriptor>      m_descriptor;
		TBAlignedArray<RHIPipeline*>		m_render_pipelines;
		Framebuffer     m_framebuffer;//每个窗口一个
	};
}