#include "vullkan_rhi_resource.h"
#include "Components/GeometryComponent.h"
#include <array>
VkFormat TourBillon::VulkanRHIResource::RHIToVkFormat(const RHIFormat& rhi_format)
{
	switch (rhi_format)
	{
	case RHIFormat::RHI_Format_A8R8G8B8:

		break;
	default:
		break;
	}

    return VkFormat();
}

VkAttachmentLoadOp TourBillon::VulkanRHIResource::RHIToVkAttachmentLoadOp(const RHIRTLoadActions& rhi_format)
{
	switch (rhi_format)
	{
	case RHIRTLoadActions::Load_NoAction:
		return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	case  RHIRTLoadActions::Load_Load:        // 加载现有内容，用于增量更新或继续操作
		return VK_ATTACHMENT_LOAD_OP_LOAD;
	case  RHIRTLoadActions::Load_Clear:      // 清除目标内容（通常用清除颜色或深度）
		return VK_ATTACHMENT_LOAD_OP_CLEAR;
	default:
		break;
	}

	return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
}


VkAttachmentStoreOp TourBillon::VulkanRHIResource::RHIToVkAttachmentStoreOp(const RHIRTStoreActions& rhi_format)
{
	switch (rhi_format)
	{
	case RHIRTStoreActions::Store_NoAction:
		return VK_ATTACHMENT_STORE_OP_DONT_CARE;
	case RHIRTStoreActions::Store_Store:
		return VK_ATTACHMENT_STORE_OP_STORE;
	default:
		break;
	}

	return VK_ATTACHMENT_STORE_OP_DONT_CARE;
}

VkPipelineBindPoint TourBillon::VulkanRHIResource::RHIToVkPipelineBindPoint(const RHIPipelineState& rhi_format)
{
	switch (rhi_format)
	{
	case RHIPipelineState::PipelineStage_GRAPHICS:
		return VK_PIPELINE_BIND_POINT_GRAPHICS;
	case RHIPipelineState::PipelineStage_COMPUTE:
		return VK_PIPELINE_BIND_POINT_COMPUTE;
	default:
		break;
	}

	return VK_PIPELINE_BIND_POINT_GRAPHICS;
}

VkShaderStageFlagBits TourBillon::VulkanRHIResource::RHIToVkShaderStageFlagBits(const RHIShderType& rhi_format)
{
	switch (rhi_format)
	{
	case RHIShderType::ShaderType_Frag:
		return VK_SHADER_STAGE_FRAGMENT_BIT;
	case RHIShderType::ShaderType_Vert:
		return VK_SHADER_STAGE_VERTEX_BIT;
	default:
		break;
	}

	return VK_SHADER_STAGE_COMPUTE_BIT;
}

