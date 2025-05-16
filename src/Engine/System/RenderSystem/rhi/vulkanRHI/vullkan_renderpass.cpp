#include "vullkan_pipeline.h"
#include "ReadFile.hpp"
#include "vullkan_renderpass.h"

TourBillon::VulkanRenderPass::~VulkanRenderPass()
{
}

bool TourBillon::VulkanRenderPass::createRenderPass(const VulkanRenderPassCreateInfo* pCreateInfo)
{
    // attachment convert
    TBAlignedArray<VkAttachmentDescription> attachments(pCreateInfo->attachments.size());
    TBAlignedArray<VkAttachmentReference> attachmentRefs(pCreateInfo->attachments.size());
    for (int i = 0; i < pCreateInfo->attachments.size(); i++)
    {
        VulkanRenderPassCreateInfo::AttachmentEntry color_entry = pCreateInfo->attachments[i];

        //主要由附件类型决定，因此不需要暴露rhi接口
        VkAttachmentDescription itAttachment{};
        itAttachment.flags = i;
        itAttachment.format = color_entry.rendertargetFormat;
        itAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        itAttachment.loadOp = color_entry.LoadAction;
        itAttachment.storeOp = color_entry.StoreAction;
        itAttachment.stencilLoadOp = color_entry.stencilLoadAction;
        itAttachment.stencilStoreOp = color_entry.stencilStoreAction;
        itAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        itAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        attachments[i] = itAttachment;

        VkAttachmentReference itAttachmentRef{};
        itAttachmentRef.attachment = i;
        itAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;//

        attachmentRefs[i] = itAttachmentRef;
    }
    TBAlignedArray<VkSubpassDescription> subpasses(pCreateInfo->subpasses.size());
    TBAlignedArray<void*> sub_attachments_ref_copy(pCreateInfo->subpasses.size());
    for (int i = 0; i < pCreateInfo->subpasses.size(); i++)
    {
        VulkanRenderPassCreateInfo::SubpassEntry subpass_entry = pCreateInfo->subpasses[i];

        for (int index = 0; index < subpass_entry.colorAttachment.size(); index++)
        {
            sub_attachments_ref_copy[index] = (void*)&attachmentRefs[subpass_entry.colorAttachment[index]];
        }

        VkSubpassDescription itsubpass{};
        itsubpass.pipelineBindPoint = subpass_entry.bindPipelineState;// VK_PIPELINE_BIND_POINT_GRAPHICS;
        itsubpass.colorAttachmentCount = sub_attachments_ref_copy.size();
        itsubpass.pColorAttachments = (VkAttachmentReference*)*sub_attachments_ref_copy.data();//内存泄露

        subpasses[i] = itsubpass;

    }

    uint32_t viewMasks[2] = { 0b11, 0b10 };//多个窗口下用可变数组，左移1位生产mask

    VkRenderPassMultiviewCreateInfo multiviewCreateInfo = {};
    multiviewCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO;
    multiviewCreateInfo.subpassCount = 2;
    multiviewCreateInfo.pViewMasks = viewMasks;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = subpasses.size();
    renderPassInfo.pSubpasses = subpasses.data();
    renderPassInfo.pNext = subpasses.data();

    VulkanRenderPass* vk_render_pass = new VulkanRenderPass;//dynamic_cast<VulkanRenderPass*>(pRenderPass);

    std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();

    if (vkCreateRenderPass(vkrhi->getDevice(), &renderPassInfo, nullptr, &m_renderpass) != VK_SUCCESS) {
        LOG_WARNING("failed to create render pass!");
        return false;
    }
    return true;
}
