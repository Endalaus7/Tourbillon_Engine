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
		// ��Ⱦ������Ϣ.
		struct AttachmentEntry
		{
			RHICommonFlags layoutRef;

			RHIRTLoadActions  LoadAction;//���ز���
			RHIRTStoreActions StoreAction;//�洢����
			RHIRTLoadActions  stencilLoadAction;//���ز���
			RHIRTStoreActions stencilStoreAction;//�洢����
		};
		TBAlignedArray<AttachmentEntry> attachments;

		struct SubpassEntry
		{
			TBAlignedArray<size_t> colorAttachment;
			RHIPipelineState bindPipelineState;
			//dependency
		};
		TBAlignedArray<SubpassEntry> subpasses;

		// ������renderpassӦ���Ƕ���ͼ������Ҫ������ͼ.
		uint8_t MultiViewCount = 0;


		// �Ƿ�̫��UAV.
		bool bTooManyUAVs = false;//overlap
		bool bIsMSAA = false;

		// ����RHI����ʾ����Ⱦͨ�������ض�����ͨ��.
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
		TBVector<RHIDrawMeshInfo> drawMeshinfos;//��TBAlignedArray
	};
	
}