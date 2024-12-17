#pragma once
#include "render/render_pass.h"

namespace TourBillon
{
	struct RenderPassInitInfo;
	struct ColorRenderPassInitInfo:public RenderPassInitInfo
	{

	};

	class ColorRenderPass :public RenderPass
	{
	public:
		~ColorRenderPass();
		virtual void initialize(const RenderPassInitInfo* init_info);
		virtual void postInitialize();

		virtual void drawPass(float dt, RHIDrawInfo& drawinfo)override;

		virtual void setup_DescriptorSetLayout()override;
		virtual void setup_RenderPass()override;
		virtual void setup_Pipeline()override;
		virtual void setup_FrameBuffer()override;
	private:
	};
}