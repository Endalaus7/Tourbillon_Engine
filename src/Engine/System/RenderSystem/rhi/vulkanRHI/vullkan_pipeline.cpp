#include "vullkan_pipeline.h"
#include "ReadFile.hpp"
#include "vullkan_renderpass.h"
TourBillon::VulkanRenderPipeline::~VulkanRenderPipeline()
{
}


bool TourBillon::VulkanRenderPipeline::createGraphicsPipeline(const VulkanPipelineCreateInfo* pCreateInfo)
{
    std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();

    //VulkanPipeline* vk_pipeline = new VulkanPipeline;// dynamic_cast<VulkanPipeline*>(pPipeline);

    TBAlignedArray<VkPipelineShaderStageCreateInfo> shaderStages;
    shaderStages.apply(pCreateInfo->shaders.size());
    std::vector<VkShaderModule> loading_shaderModules(pCreateInfo->shaders.size());
    for (int i = 0; i < pCreateInfo->shaders.size(); i++)
    {
        VulkanPipelineCreateInfo::ShaderEntry shader_entry = pCreateInfo->shaders[i];
        auto shaderCode = readFile(shader_entry.shaderpath);
        VkShaderModule shaderModule = vkrhi->createShaderModule(shaderCode);
        
        VkPipelineShaderStageCreateInfo shaderStageInfo{};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = VulkanRHIResource::RHIToVkShaderStageFlagBits(shader_entry.shadertype);
        shaderStageInfo.module = shaderModule;
        shaderStageInfo.pName = "main";

        loading_shaderModules[i] = shaderModule;
        shaderStages[i] = shaderStageInfo;
    }

    //绑定描述符
    VkVertexInputBindingDescription bindingDescription = getVertexBindingDescription();
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions = getVertexAttributeDescriptions();

    //定义顶点输入的格式和绑定方式
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();


    //顶点组装的方式
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    //视口和剪裁区域
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    //深度测试
    VkPipelineDepthStencilStateCreateInfo depth_info = {};
    depth_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_info.depthCompareOp = VK_COMPARE_OP_ALWAYS;

    //光栅化状态
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    //多重采样的配置
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    //颜色混合的配置
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    //在管线外动态设置的状态
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = pCreateInfo->descriptor_layouts.size();
    //pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    TBVector<VkDescriptorSetLayout> vk_layouts(pCreateInfo->descriptor_layouts.size());
    for (int i = 0; i < pCreateInfo->descriptor_layouts.size(); i++)
    {
        vk_layouts[i] = dynamic_cast<VulkanDescriptorSetLayout*>(pCreateInfo->descriptor_layouts[i])->layout;
    }
    pipelineLayoutInfo.pSetLayouts = vk_layouts.data();

    if (vkCreatePipelineLayout(vkrhi->getDevice(), &pipelineLayoutInfo, nullptr, &m_pipelinelayout) != VK_SUCCESS) {
        LOG_WARNING("failed to create pipeline layout!");
    }

    VulkanRenderPass* vk_render_pass = dynamic_cast<VulkanRenderPass*>(pCreateInfo->renderpass);

    //VkPipelineRobustnessBufferBehaviorCreateInfoEXT robustnessBufferBehavior = {};
    //robustnessBufferBehavior.sType = VK_STRUCTURE_TYPE_PIPELINE_ROBUSTNESS_BUFFER_BEHAVIOR_CREATE_INFO_EXT;
    //robustnessBufferBehavior.bufferAccessBehavior = VK_PIPELINE_ROBUSTNESS_BUFFER_ACCESS_ROBUST_BUFFER_ACCESS_EXT;


    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = pCreateInfo->shaders.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDepthStencilState = &depth_info;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = m_pipelinelayout;
    pipelineInfo.renderPass = vk_render_pass->getRenderPass();
    pipelineInfo.subpass = pCreateInfo->subpassIndex;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();
    if (vkCreateGraphicsPipelines(vkrhi->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS) {
        LOG_ERROR("failed to create graphics pipeline!");
        return false;
    }
    for (int i = 0; i < loading_shaderModules.size(); i++)
    {
        vkDestroyShaderModule(vkrhi->getDevice(), loading_shaderModules[i], nullptr);
    }
    return true;
}
