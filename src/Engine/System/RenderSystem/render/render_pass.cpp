#include "render_pass.h"
#include "log.hpp"
void TourBillon::RenderPass::initialize(const RenderPassInitInfo* init_info)
{
	m_rhi = init_info->rhi;
	if (!m_rhi)
		LOG_ERROR("RHI is empty");

	setup_DescriptorSetLayout();
	setup_RenderPass();
	setup_Pipeline();
	setup_FrameBuffer();
}

void TourBillon::RenderPass::postInitialize()
{
}
