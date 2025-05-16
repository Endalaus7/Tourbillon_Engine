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

//vulkan renderpass�İ�װ
//����rhi��Դ��������ecs��Դ�����������ָ����ʹ���ĸ�renderpass����



namespace TourBillon
{
	struct VulkanRenderPassCreateInfo
	{
		struct AttachmentEntry
		{
			//RHICommonFlags layoutRef;
			VkFormat rendertargetFormat;

			VkAttachmentLoadOp  LoadAction;//���ز���
			VkAttachmentStoreOp StoreAction;//�洢����
			VkAttachmentLoadOp  stencilLoadAction;//���ز���
			VkAttachmentStoreOp stencilStoreAction;//�洢����
		};
		TBAlignedArray<AttachmentEntry> attachments;

		struct SubpassEntry
		{
			TBAlignedArray<size_t> colorAttachment;
			VkPipelineBindPoint bindPipelineState;
			//dependency
		};
		TBAlignedArray<SubpassEntry> subpasses;//ÿ���������һ��subpass

		// ������renderpassӦ���Ƕ���ͼ������Ҫ������ͼ.
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