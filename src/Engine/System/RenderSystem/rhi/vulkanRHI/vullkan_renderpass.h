#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
//#include "rhi/rhi_renderpass.h"
#include <vector>
#include <array>
#include "common.hpp"
#include "ECSCommon.h"
#include "Structure/TB_AlignedArray.hpp"
#include "vullkan_rhi_resource.h"
#include "vullkan_rhi.h"

//vulkan renderpass的包装
//属于rhi资源，不属于ecs资源，窗口组件中指定其使用哪个renderpass即可



namespace TourBillon
{
	struct VulkanRenderPassCreateInfo
	{
		struct AttachmentEntry
		{
			//RHICommonFlags layoutRef;
			VkFormat rendertargetFormat;

			VkAttachmentLoadOp  LoadAction;//加载操作
			VkAttachmentStoreOp StoreAction;//存储操作
			VkAttachmentLoadOp  stencilLoadAction;//加载操作
			VkAttachmentStoreOp stencilStoreAction;//存储操作
		};
		TBAlignedArray<AttachmentEntry> attachments;

		struct SubpassEntry
		{
			TBAlignedArray<size_t> colorAttachment;
			VkPipelineBindPoint bindPipelineState;
			//dependency
		};
		TBAlignedArray<SubpassEntry> subpasses;//每个相机持有一个subpass

		// 如果这个renderpass应该是多视图，则需要多少视图.
		uint8_t MultiViewCount = 0;

		bool bIsMSAA = false;
	};

    class VulkanRenderPass
    {
    public:
        ~VulkanRenderPass();
        //virtual void initialize(RHIRenderPassInfo& initialize_info) override;
        bool createRenderPass(const VulkanRenderPassCreateInfo* pCreateInfo);

		FORCE_INLINE VkRenderPass getRenderPass() { return m_renderpass; }
    private:
        //std::shared_ptr<VulkanRHI> m_vkrhi;

        VkRenderPass m_renderpass;
    };
}