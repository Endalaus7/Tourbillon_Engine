#pragma once

#include "rhi/rhi_struct.h"
#include "rhi/rhi_type.h"
#include "rhi/rhi_source.h"

#include <vulkan/vulkan.h>
#include <optional>

#include "components/GeometryComponent.h"

//负责将rhi资源转译成vulkan资源

#define GENERATE_VK_RHI_RESOURCE(VulkanType, RHIType)				\
public:                                                             \
    static VulkanType RHITo##VulkanType(const RHIType& value);				

#define GET_VULKAN_RHI() std::dynamic_pointer_cast<VulkanRHI>(RHI_Factory::Instance()->getRHI(VULKAN_RHI))


namespace TourBillon
{
	class VulkanRHIResource {

	public:
		GENERATE_VK_RHI_RESOURCE(VkFormat, RHIFormat)
		GENERATE_VK_RHI_RESOURCE(VkAttachmentLoadOp, RHIRTLoadActions)
		GENERATE_VK_RHI_RESOURCE(VkAttachmentStoreOp, RHIRTStoreActions)
		GENERATE_VK_RHI_RESOURCE(VkPipelineBindPoint, RHIPipelineState)
		GENERATE_VK_RHI_RESOURCE(VkShaderStageFlagBits, RHIShderType)
	public:
	};

	//struct VulkanRenderPass:public RHIRenderPass
	//{
	//	VkRenderPass renderpass;
	//};
	struct VulkanPipeline :public RHIPipeline
	{
		VkPipeline pipeline;
		VkPipelineLayout pipelinelayout;
	};
	struct VulkanCommandBuffer :public RHICommandBuffer
	{
		VkCommandBuffer commandbuffer;
	};
	struct VulkanFramebuffer :public RHIFramebuffer
	{
		VkFramebuffer framebuffer;
	};
	struct VulkanImage :public RHIImage
	{
		VkImage image;
	};
	struct VulkanImageView :public RHIImageView
	{
		VkImageView imageview;
	};
	struct VulkanDescriptorSet :RHIDescriptorSet
	{
		VkDescriptorSet descriptor;
	};
	struct VulkanDescriptorSetLayout :public RHIDescriptorSetLayout
	{
		VkDescriptorSetLayout layout;
	};
	struct VulkanBuffer :public RHIBuffer
	{
		VkBuffer buffer;
	};
	struct VulkanDeviceMemory :public RHIDeviceMemory
	{
		VkDeviceMemory devicememory;
	};
	
	struct VulkanSampler :public RHISampler
	{
		VkSampler sampler;
	};

	//改为动态
	static VkVertexInputBindingDescription getVertexBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	//改为动态
	static std::vector<VkVertexInputAttributeDescription> getVertexAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(Vertex::_field_count_);

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, Vertex::pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, Vertex::uv);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, Vertex::color);

		return attributeDescriptions;
	}
}