#pragma once

#include "rhi_type.h"
#include "event.hpp"
#include "common.hpp"
#include "Structure/TB_AlignedArray.hpp"
#define DEF_RHI_STRUCT_INTERFACE(structType)\
class structType{ public: virtual ~##structType(){}};

namespace TourBillon
{
	DEF_RHI_STRUCT_INTERFACE(RHIRenderPass)
		DEF_RHI_STRUCT_INTERFACE(RHIPipeline)
		DEF_RHI_STRUCT_INTERFACE(RHIShader)
		DEF_RHI_STRUCT_INTERFACE(RHIFramebuffer)
		DEF_RHI_STRUCT_INTERFACE(RHIImage)
		DEF_RHI_STRUCT_INTERFACE(RHIDeviceMemory)
		DEF_RHI_STRUCT_INTERFACE(RHIImageView)
		DEF_RHI_STRUCT_INTERFACE(RHIDescriptorSetLayout)
		DEF_RHI_STRUCT_INTERFACE(RHIDescriptorSet)
		DEF_RHI_STRUCT_INTERFACE(RHICommandBuffer)
		DEF_RHI_STRUCT_INTERFACE(RHIBuffer)




		struct RHIRenderPassCreateInfo
	{
		// 渲染纹理信息.
		struct AttachmentEntry
		{
			RHICommonFlags layoutRef;

			RHIRTLoadActions  LoadAction;//加载操作
			RHIRTStoreActions StoreAction;//存储操作
			RHIRTLoadActions  stencilLoadAction;//加载操作
			RHIRTStoreActions stencilStoreAction;//存储操作
		};
		TBAlignedArray<AttachmentEntry> attachments;

		struct SubpassEntry
		{
			TBAlignedArray<size_t> colorAttachment;
			RHIPipelineState bindPipelineState;
			//dependency
		};
		TBAlignedArray<SubpassEntry> subpasses;

		// 如果这个renderpass应该是多视图，则需要多少视图.
		uint8_t MultiViewCount = 0;


		// 是否太多UAV.
		bool bTooManyUAVs = false;//overlap
		bool bIsMSAA = false;

		// 部分RHI的提示，渲染通道将有特定的子通道.
		//RHISubPassHint SubpassHint = RHISubPassHint::None;

	};

	struct RHIDescriptorSetLayoutBinding
	{
		uint32_t binding;
		RHIDescriptorType descriptorType;
		uint32_t descriptorCount;
		RHIShderType stageFlags;
	};
	struct RHIDescriptorSetLayoutCreateInfo {
		uint32_t bindingCount;
		const RHIDescriptorSetLayoutBinding* pBindings;
	};
	struct RHIDescriptorSetAllocateInfo {
		RHIDescriptorSetLayout* layout;
	};
	struct RHIWriteDescriptorSetInfo
	{
		RHIBuffer* buffer;
		RHIDescriptorSet* descriptorset;
		RHIDeviceSize range;
		RHIDescriptorType descriptorType;
	};
	struct RHIUpdatesDescriptorSetsInfo
	{
		TBVector<RHIWriteDescriptorSetInfo> write_info;

	};

	struct RHIPipelineCreateInfo {
		struct ShaderEntry
		{
			RHIShderType shadertype;
			RHIPath shaderpath;
		};
		TBAlignedArray<ShaderEntry> shaders;

		RHIRenderPass* renderpass;
		uint32_t subpassIndex;
		TBVector<RHIDescriptorSetLayout*> descriptor_layouts;
	};
	struct RHIFramebufferCreateInfo {
		RHIRenderPass* renderpass;
		TBAlignedArray<RHIImageView*> imageviews;
	};

	class RHIBufferResource;
	struct RHIDrawMeshInfo
	{
		virtual ~RHIDrawMeshInfo() {}

		uint32_t indices_count;
		RHIBufferResource* vertex_buffer;
		RHIBufferResource* index_buffer;
	};

	struct RHIDrawInfo
	{
		virtual ~RHIDrawInfo() {}
		void reset()
		{
			pipeline = nullptr;
			renderpass = nullptr;
			framebuffers = nullptr;
		}

		RHIPipeline* pipeline;
		RHIRenderPass* renderpass;
		RHIFramebuffer* framebuffers;
		CEvent preEvents;
		CEvent subEvents;
		CEvent drawEvents;
		TBVector<RHIDescriptorSet*> descriptor_sets;
		TBVector<uint32_t> uboDynamicOffsets;
		TBVector<RHIDrawMeshInfo> drawMeshinfos;//换TBAlignedArray
	};
	
}