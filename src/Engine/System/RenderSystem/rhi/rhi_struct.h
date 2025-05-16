#pragma once

#include "rhi_type.h"
#include "event.hpp"
#include "common.hpp"
#include "Structure/TB_AlignedArray.hpp"
#define DEF_RHI_STRUCT_INTERFACE(structType)\
class structType{ public: virtual ~##structType(){}};

namespace TourBillon
{
	//DEF_RHI_STRUCT_INTERFACE(RHIRenderPass)
		DEF_RHI_STRUCT_INTERFACE(RHIPipeline)
		DEF_RHI_STRUCT_INTERFACE(RHIShader)
		DEF_RHI_STRUCT_INTERFACE(RHIFramebuffer)
		DEF_RHI_STRUCT_INTERFACE(RHIImage)
		DEF_RHI_STRUCT_INTERFACE(RHISampler)
		DEF_RHI_STRUCT_INTERFACE(RHIDeviceMemory)
		DEF_RHI_STRUCT_INTERFACE(RHIImageView)
		DEF_RHI_STRUCT_INTERFACE(RHIDescriptorSetLayout)
		DEF_RHI_STRUCT_INTERFACE(RHIDescriptorSet)
		DEF_RHI_STRUCT_INTERFACE(RHICommandBuffer)
		DEF_RHI_STRUCT_INTERFACE(RHIBuffer)

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
		uint32_t bindingindex;
		RHIBuffer* buffer = nullptr;
		RHIImageView* imageview = nullptr;
		RHISampler* sampler = nullptr;
		RHIDescriptorSet* descriptorset = nullptr;
		RHIDeviceSize range;
		RHIDeviceSize offset;
		RHIDescriptorType descriptorType;
	};
	struct RHIUpdatesDescriptorSetsInfo
	{
		TBVector<RHIWriteDescriptorSetInfo> write_info;//

	};
	struct RHICreateTextureSamplerInfo {
		RHISamplerFilter minFilter;
		RHISamplerFilter magFilter; 
		RHISamplerAddressMode addressmode; 
		RHIMipmapMode mipmapmode;
		bool anisotropyEnable;
	};
	
	

	class RHIBufferResource;
	struct RHIDrawMeshInfo
	{
		virtual ~RHIDrawMeshInfo() {}

		uint32_t indices_count;
		RHIBufferResource* vertex_buffer;
		RHIBufferResource* index_buffer;

		
		TBVector<uint32_t> uboDynamicOffsets;//偏移量
	};

	struct RHIDrawInfo
	{
		virtual ~RHIDrawInfo() {}
		void reset()
		{
			pipeline = nullptr;
			renderpass = nullptr;
			framebuffers = nullptr;
			//descriptor_sets.clear();
		}

		//uint32_t windowIndex = 0;

		RHIPipeline* pipeline;
		RHIRenderPass* renderpass;
		RHIFramebuffer* framebuffers;
		CEvent resizeRenderEvents;
		CEvent drawEvents;
		
		TBVector<RHIDrawMeshInfo> drawMeshinfos;//换TBAlignedArray,每种mesh只要一份
		TBVector<RHIDescriptorSet*> descriptor_sets;//描述符集
	};
	
}