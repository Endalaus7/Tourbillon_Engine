#include "main_camera_pass.h"
#include <array>
#include <vulkan/vulkan_core.h>
#include "Components/GeometryComponent.h"
#include "Components/CameraComponent.h"

TourBillon::MainCameraPass::~MainCameraPass()
{
	//vk方面的东西要清理

	SAFE_DELETE(m_uniformbuffer);
	SAFE_DELETE(m_uniformdynamicbuffer);
}
void TourBillon::MainCameraPass::initialize(const RenderPassInitInfo* init_info)
{
	RenderPass::initialize(init_info);

	const MainCameraPassInitInfo* _init_info = static_cast<const MainCameraPassInitInfo*>(init_info);
	//m_framebuffer.render_pass = _init_info->render_pass;



	m_uniformbuffer = new RHIBufferResource;
	m_uniformdynamicbuffer = new RHIBufferResource;

	//updateUboData();
	//uint64_t uboDynamicBufferSize = static_cast<uint64_t>(m_uniform_buffer_dynamic_object_cache.size() * sizeof(UniformBufferDynamicObject));
	//m_rhi->createUniformBuffer((void*)&m_uniform_buffer_object, sizeof(UniformBufferObject), m_uniformbuffer->buffer, m_uniformbuffer->buffermemory);
	//m_rhi->createUniformBuffer((void*)m_uniform_buffer_dynamic_object_cache.data(), sizeof(UniformBufferDynamicObject), m_uniformdynamicbuffer->buffer, m_uniformdynamicbuffer->buffermemory);
	//
	//
	//updateDescriptorSet();
}

void TourBillon::MainCameraPass::postInitialize()
{

}

void TourBillon::MainCameraPass::beforeDraw(float dt, RHIDrawInfo& drawinfo)
{
	updateUboData();
	//updateUniformDescriptorSet();
}

void TourBillon::MainCameraPass::drawPass(float dt, RHIDrawInfo& drawinfo)
{
	updateUniformUboData(drawinfo.windowIndex);
	

	drawinfo.pipeline = m_render_pipelines[0];
	drawinfo.renderpass = m_framebuffer.render_pass;
	drawinfo.framebuffers = m_framebuffer.framebuffers[drawinfo.windowIndex][m_rhi->getCurrentFrameIndex()];

	drawinfo.descriptor_sets.resize(m_descriptor.size());
	for (int i=0;i< m_descriptor.size();i++)
	{
		drawinfo.descriptor_sets[i] = m_descriptor[i].descriptor_set;
		
	}

	const auto& trans_components = ECSManager::Instance()->GetComponentEntities<Transfrom>();
	drawinfo.uboDynamicOffsets.resize(trans_components.size());
	for (int index = 0; index < trans_components.size(); index++)
	{
		drawinfo.uboDynamicOffsets[index] = index * sizeof(UniformBufferDynamicObject);
	}
	//drawinfo.descriptor_sets = m_descriptor.descriptor_sets;

	updateUboBuffer(drawinfo);

	//updateDescriptorSet();
	drawinfo.drawEvents.addCallback([&](const CEvent&) {

		//for (auto& drawmeshinfo : drawinfo.drawMeshinfos)
		{
			m_rhi->DrawMesh(drawinfo, drawinfo.drawMeshinfos[0]);
		}

		});

	m_rhi->UpdateDraw(dt, drawinfo);
}

void TourBillon::MainCameraPass::updateDescriptorSets(float dt, RHIDrawInfo& drawinfo)
{
	updateDescriptorSet();
}

void TourBillon::MainCameraPass::setup_DescriptorSetLayout()
{
	///m_descriptor_infos.apply(1);
	setupDescriptorSet();
}

void TourBillon::MainCameraPass::setup_RenderPass()
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

void TourBillon::MainCameraPass::setup_Pipeline()
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

	TBVector<RHIDescriptorSetLayout*> layouts(m_descriptor.size());
	for (int i = 0;i< m_descriptor.size();i++)
	{
		layouts[i] = m_descriptor[i].layout;
	}
	rhi_pipeline_create_info.descriptor_layouts = layouts;

	RHIPipeline* rhi_create_pipeline;
	m_rhi->createGraphicsPipeline(&rhi_pipeline_create_info, rhi_create_pipeline);
	m_render_pipelines.apply(1);
	m_render_pipelines[0] = rhi_create_pipeline;
}

void TourBillon::MainCameraPass::setup_FrameBuffer()
{
	RHIFramebufferCreateInfo rhi_framebuffer_init_info;
	uint32_t windowsize = m_rhi->m_windowSize;
	m_framebuffer.framebuffers.resize(windowsize);
	for (int iwin = 0; iwin < windowsize; iwin++)
	{
		auto swapimageviews = m_rhi->getSwapChainImageViews(iwin);
		m_framebuffer.framebuffers[iwin].apply(swapimageviews.size());
		for (int i = 0; i < swapimageviews.size(); i++)
		{
			rhi_framebuffer_init_info.renderpass = m_framebuffer.render_pass;
			rhi_framebuffer_init_info.windowIndex = iwin;
			rhi_framebuffer_init_info.imageviews.apply(1);
			rhi_framebuffer_init_info.imageviews[0] = swapimageviews[i];
			m_rhi->createFrameBuffer(&rhi_framebuffer_init_info, m_framebuffer.framebuffers[iwin][i]);
		}
	}
}

void TourBillon::MainCameraPass::destroyFramebuffer()
{
	for(auto& windowframes:m_framebuffer.framebuffers)
	{
		for(auto frame: windowframes)
			m_rhi->destroyFramebuffer(frame);
	}
}

void TourBillon::MainCameraPass::cacheUniformObject(const TBMath::Mat44& proj_view_matrix)
{
	m_uniform_buffer_object.proj_view_matrix = proj_view_matrix;
}

void TourBillon::MainCameraPass::cacheUniformDynamicObject()
{
	size_t trans_obj_count = ECSManager::Instance()->GetComponentSize<Transfrom>();
	m_uniform_buffer_dynamic_object_cache.resize(trans_obj_count);

	const auto& trans_components = ECSManager::Instance()->GetComponentEntities<Transfrom>();
	size_t index = 0;
	for (auto entity : trans_components)
	{
		auto& dyn_trans = ECSManager::Instance()->GetComponent<Transfrom>(entity);

		m_uniform_buffer_dynamic_object_cache[index].model_matrix = dyn_trans.GetModelMatrix();
		index++;
	}
}

void TourBillon::MainCameraPass::setMainCamera(uint32_t windowindex, Entity camera)
{
	if (windowindex >= m_camera.size())
		m_camera.resize(windowindex + 1);
	m_camera[windowindex] = camera;
}

void TourBillon::MainCameraPass::updateUboData()
{
	cacheUniformDynamicObject();
	//clear vk resource
	if (m_renderCount != ECSManager::Instance()->GetComponentSize<Transfrom>())
	{
		dirtyUniformBuffer();
	}
}

void TourBillon::MainCameraPass::updateUniformUboData(uint32_t windowindex)
{
	Camera3D& camera = ECSManager::Instance()->GetComponent<Camera3D>(m_camera[windowindex]);
	cacheUniformObject(camera.GetVPMatrix());
}

void TourBillon::MainCameraPass::updateUboBuffer(RHIDrawInfo& info)
{
	m_rhi->updateBuffer((void*)&m_uniform_buffer_object, info.windowIndex, m_uniformbuffer->buffer, 0, sizeof(UniformBufferObject));
	m_rhi->updateBuffer((void*)m_uniform_buffer_dynamic_object_cache.data(), info.windowIndex, m_uniformdynamicbuffer->buffer, 0, sizeof(UniformBufferDynamicObject) * m_uniform_buffer_dynamic_object_cache.size());
}

void TourBillon::MainCameraPass::updateDescriptorSet()
{
	RHIUpdatesDescriptorSetsInfo update_descriptorsets_info;
	update_descriptorsets_info.write_info.resize(2);

	update_descriptorsets_info.write_info[0].bindingindex = 0;
	update_descriptorsets_info.write_info[0].buffer = m_uniformbuffer->buffer;
	update_descriptorsets_info.write_info[0].descriptorset = m_descriptor[m_rhi->getCurrentFrameIndex()].descriptor_set;
	update_descriptorsets_info.write_info[0].range = sizeof(UniformBufferObject);
	update_descriptorsets_info.write_info[0].offset = 0;
	update_descriptorsets_info.write_info[0].descriptorType = RHI_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	update_descriptorsets_info.write_info[1].bindingindex = 1;
	update_descriptorsets_info.write_info[1].buffer = m_uniformdynamicbuffer->buffer;
	update_descriptorsets_info.write_info[1].descriptorset = m_descriptor[m_rhi->getCurrentFrameIndex()].descriptor_set;
	update_descriptorsets_info.write_info[1].range = sizeof(UniformBufferDynamicObject);// *m_uniform_buffer_dynamic_object_cache.size();
	update_descriptorsets_info.write_info[1].offset = 0;
	update_descriptorsets_info.write_info[1].descriptorType = RHI_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	m_rhi->updateDescriptorSets(update_descriptorsets_info);
}

void TourBillon::MainCameraPass::dirtyUniformBuffer()
{
	uint64_t uboDynamicBufferSize = static_cast<uint64_t>(m_uniform_buffer_dynamic_object_cache.size() * sizeof(UniformBufferDynamicObject));
	m_rhi->createUniformBuffer((void*)&m_uniform_buffer_object, sizeof(UniformBufferObject), m_uniformbuffer->buffer, m_uniformbuffer->buffermemory);
	m_rhi->createUniformBuffer((void*)m_uniform_buffer_dynamic_object_cache.data(), sizeof(UniformBufferDynamicObject)* m_uniform_buffer_dynamic_object_cache.size(), m_uniformdynamicbuffer->buffer, m_uniformdynamicbuffer->buffermemory);
	m_renderCount = ECSManager::Instance()->GetComponentSize<Transfrom>();
	
}

void TourBillon::MainCameraPass::setupDescriptorSet()
{
	RHIDescriptorSetLayoutBinding mesh_uboLayoutBinding[2];

	mesh_uboLayoutBinding[0].binding = 0;
	mesh_uboLayoutBinding[0].descriptorType = RHI_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	mesh_uboLayoutBinding[0].descriptorCount = 1;
	mesh_uboLayoutBinding[0].stageFlags = ShaderType_Vert;

	mesh_uboLayoutBinding[1].binding = 1;
	mesh_uboLayoutBinding[1].descriptorType = RHI_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	mesh_uboLayoutBinding[1].descriptorCount = 1;
	mesh_uboLayoutBinding[1].stageFlags = ShaderType_Vert;

	RHIDescriptorSetLayoutCreateInfo setlayout_create_info;
	setlayout_create_info.bindingCount = 2;
	setlayout_create_info.pBindings = mesh_uboLayoutBinding;

	m_descriptor.resize(m_rhi->getMaxFrameIndex());
	//m_descriptor.resize();

	for (int i = 0; i < m_descriptor.size(); i++)
	{
		m_rhi->createDescriptorSetLayout(&setlayout_create_info, m_descriptor[i].layout);
		RHIDescriptorSetAllocateInfo setlayout_alloc_info;
		setlayout_alloc_info.layout = m_descriptor[i].layout;
		m_rhi->AllocateDescriptorSets(&setlayout_alloc_info, m_descriptor[i].descriptor_set);
	}

	//updateDescriptorSet();
}
