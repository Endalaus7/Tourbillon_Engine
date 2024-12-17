#include "color_render_pass.h"
#include <array>
#include <vulkan/vulkan_core.h>
TourBillon::ColorRenderPass::~ColorRenderPass()
{
	//vk方面的东西要清理
}
void TourBillon::ColorRenderPass::initialize(const RenderPassInitInfo* init_info)
{
	RenderPass::initialize(init_info);

	const ColorRenderPassInitInfo* _init_info = static_cast<const ColorRenderPassInitInfo*>(init_info);
	//m_framebuffer.render_pass = _init_info->render_pass;

	
}

void TourBillon::ColorRenderPass::postInitialize()
{

}

void TourBillon::ColorRenderPass::drawPass(float dt, RHIDrawInfo& drawinfo)
{
	drawinfo.pipeline = m_render_pipelines[0];
	drawinfo.renderpass = m_framebuffer.render_pass;
	drawinfo.framebuffers = m_framebuffer.framebuffers[m_rhi->getCurrentFrameIndex()];
	
	drawinfo.drawEvents.addCallback([&](const CEvent&) {

		for (auto& drawmeshinfo : drawinfo.drawMeshinfos)
		{
			m_rhi->DrawMesh(drawinfo, drawmeshinfo);
		}

		});

	m_rhi->UpdateDraw(dt, drawinfo);
}

void TourBillon::ColorRenderPass::setup_DescriptorSetLayout()
{
	///m_descriptor_infos.apply(1);
}

void TourBillon::ColorRenderPass::setup_RenderPass()
{
	RHIRenderPassCreateInfo renderpass_create_info;
	RHIRenderPassCreateInfo::AttachmentEntry attachment_create_info;
	renderpass_create_info.attachments.apply(1);
	renderpass_create_info.attachments[0].LoadAction = RHIRTLoadActions::Load_Clear;//加载操作 = 0;
	renderpass_create_info.attachments[0].StoreAction = RHIRTStoreActions::Store_Store;//存储操作 = 0;
	renderpass_create_info.attachments[0].stencilLoadAction = RHIRTLoadActions::Load_NoAction;//加载操作 = 0;
	renderpass_create_info.attachments[0].stencilStoreAction = RHIRTStoreActions::Store_NoAction;//存储操作 = 0;

	renderpass_create_info.subpasses.apply(1);
	renderpass_create_info.subpasses[0].colorAttachment.apply(1);
	renderpass_create_info.subpasses[0].colorAttachment[0] = 0;
	renderpass_create_info.subpasses[0].bindPipelineState = RHIPipelineState::PipelineStage_GRAPHICS;
	m_rhi->createRenderPass(&renderpass_create_info, m_framebuffer.render_pass);
}

void TourBillon::ColorRenderPass::setup_Pipeline()
{
	RHIPipelineCreateInfo rhi_pipeline_create_info;
	rhi_pipeline_create_info.renderpass = m_framebuffer.render_pass;
	rhi_pipeline_create_info.subpassIndex = 0;
	rhi_pipeline_create_info.shaders.apply(2);
	//RHIPipelineCreateInfo::ShaderEntry* debug0 = &rhi_pipeline_create_info.shaders[0];
	//RHIPipelineCreateInfo::ShaderEntry* debug1 = &rhi_pipeline_create_info.shaders[1];
	rhi_pipeline_create_info.shaders[0].shadertype = RHIShderType::ShaderType_Vert;
	rhi_pipeline_create_info.shaders[0].shaderpath = std::string(SHADER_PATH) + "testshader.vert.spv";
	
	rhi_pipeline_create_info.shaders[1].shadertype = RHIShderType::ShaderType_Frag;
	rhi_pipeline_create_info.shaders[1].shaderpath = std::string(SHADER_PATH) + "testshader.frag.spv";


	RHIPipeline* rhi_create_pipeline;
	m_rhi->createGraphicsPipeline(&rhi_pipeline_create_info, rhi_create_pipeline);
	m_render_pipelines.apply(1);
	m_render_pipelines[0] = rhi_create_pipeline;
}

void TourBillon::ColorRenderPass::setup_FrameBuffer()
{
	RHIFramebufferCreateInfo rhi_framebuffer_init_info;
	auto swapimageviews = m_rhi->getSwapChainImageViews();
	m_framebuffer.framebuffers.apply(swapimageviews.size());
	for (int i = 0; i < swapimageviews.size(); i++)
	{
		rhi_framebuffer_init_info.renderpass = m_framebuffer.render_pass;
		rhi_framebuffer_init_info.imageviews.apply(1);
		rhi_framebuffer_init_info.imageviews[0] = swapimageviews[i];
		m_rhi->createFrameBuffer(&rhi_framebuffer_init_info, m_framebuffer.framebuffers[i]);
	}
}
