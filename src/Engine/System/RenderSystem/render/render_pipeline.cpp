#include "render_pipeline.h"
#include "passes/main_camera_pass.h"
void TourBillon::RenderPipeline::initialize(RenderPipelineInitInfo init_info)
{
	m_rhi = init_info.rhi;

	m_RenderPassList.resize(Pass_Count);

	MainCameraPass* main_camera_pass = new MainCameraPass;
	RenderPassInitInfo main_camera_pass_init_info;
	main_camera_pass_init_info.rhi = m_rhi;
	main_camera_pass->initialize(&main_camera_pass_init_info);
	m_RenderPassList[Pass_MainCamera] = main_camera_pass;
	
}

void TourBillon::RenderPipeline::BeforeFrameDraw(float dt, RHIDrawInfo& drawinfo)
{
	for (auto pass : m_RenderPassList)
	{
		pass->beforeDraw(dt, drawinfo);
	}
	m_rhi->BeforeFrameDraw(dt);
}

void TourBillon::RenderPipeline::deferredRender(float dt, RHIDrawInfo& drawinfo)
{
	drawinfo.reset();
	drawinfo.resizeRenderEvents.addCallback([&](const CEvent&) {
		passUpdateAfterRecreateSwapchain();
		});

	bool recreate_swapchain = m_rhi->prepareDraw(dt, drawinfo);
	if (recreate_swapchain)
	{
		passUpdateAfterRecreateSwapchain();
	}
	for (auto pass : m_RenderPassList)
	{
		pass->updateDescriptorSets(dt, drawinfo);
	}
	//std::shared_ptr<RenderPass> curr_renderpass = std::static_pointer_cast<RenderPass>(m_RenderPassList.head());
	//while (curr_renderpass)
	for (auto pass : m_RenderPassList)
	{
		pass->drawPass(dt, drawinfo);
		//pass = std::static_pointer_cast<RenderPass>(curr_renderpass->next);
	}
	m_rhi->submitDraw(dt,drawinfo);

	
}

void TourBillon::RenderPipeline::AfterFrameDraw(float dt, RHIDrawInfo& drawinfo)
{
	m_rhi->AfterFrameDraw(dt);
}

void TourBillon::RenderPipeline::passUpdateAfterRecreateSwapchain()
{
	//std::shared_ptr<RenderPass> curr_renderpass = std::static_pointer_cast<RenderPass>(m_RenderPassList.head());
	//while (curr_renderpass)
	for (auto pass : m_RenderPassList)
	{
		pass->destroyFramebuffer();
		pass->setup_FrameBuffer();
		//pass = std::static_pointer_cast<RenderPass>(curr_renderpass->next);
	}
}

void TourBillon::RenderPipeline::SetMainCamera(uint32_t windowindex, Entity camera)
{
	MainCameraPass* main_camera_pass = dynamic_cast<MainCameraPass*>(m_RenderPassList[Pass_MainCamera]);
	if (main_camera_pass)
		main_camera_pass->setMainCamera(windowindex, camera);
}
