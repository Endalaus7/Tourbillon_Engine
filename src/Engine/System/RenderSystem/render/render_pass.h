#pragma once
#include <memory>
#include "rhi/rhi.h"
#include "Structure/TB_List.hpp"

namespace TourBillon
{
	struct RenderPassInitInfo{
		std::shared_ptr<RHI>                rhi;
		//std::shared_ptr<RenderResource> render_resource;
	};
	class RenderPipeline;
	class RenderPass:public TBListMem
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
			TBVector<TBAlignedArray<RHIFramebuffer*>> framebuffers;//每个窗口一组，一组3个
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

		virtual void drawPass(float dt, RHIDrawInfo& drawinfo) = 0;

		virtual void setup_DescriptorSetLayout() {}
		virtual void setup_RenderPass() {}
		virtual void setup_Pipeline() {}
		virtual void setup_FrameBuffer() {}
	protected:
		std::shared_ptr<RHI>                m_rhi;
		//std::shared_ptr<RenderResource> m_render_resource;

		TBVector<Descriptor>      m_descriptor;
		TBAlignedArray<RHIPipeline*>		m_render_pipelines;
		Framebuffer     m_framebuffer;//每个窗口一个
	};
}