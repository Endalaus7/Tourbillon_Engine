#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <array>
#include "common.hpp"
#include "ECSCommon.h"
#include "Structure/TB_AlignedArray.hpp"
#include "vullkan_rhi_resource.h"
#include "vullkan_rhi.h"
#include "vullkan_renderpass.h"

//��Ⱦ��һ���ӿ��ϵ�ʵ��
//ÿ��portֻ����һ������


namespace TourBillon
{
    struct VulkanPipelineCreateInfo {
        struct ShaderEntry
        {
            RHIShderType shadertype;
            RHIPath shaderpath;
        };
        TBAlignedArray<ShaderEntry> shaders;

        VulkanRenderPass* renderpass;
        uint32_t subpassIndex;
        TBVector<RHIDescriptorSetLayout*> descriptor_layouts;
    };

    class VulkanRenderPipeline
    {
    public:
        ~VulkanRenderPipeline();
        //virtual void initialize(RHIPipelineInfo& initialize_info) override;

        virtual bool createGraphicsPipeline(const VulkanPipelineCreateInfo* pCreateInfo);

    private:
        //std::shared_ptr<VulkanRHI> m_vkrhi;

        VkPipeline m_graphicsPipeline;
        VkPipelineLayout m_pipelinelayout;
    };
}