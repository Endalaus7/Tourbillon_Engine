#pragma once

#include <memory>
#include "common.hpp"
#include "log.hpp"
#include "rhi_struct.h"
#include "rhi_source.h"
#include "Structure/TB_AlignedArray.hpp"
#include "Structure/TB_List.hpp"


namespace TourBillon
{
	class RHIWindow;

	enum RHI_TYPE
	{
		VULKAN_RHI = 0x0
	};

	struct RHIInitInfo
	{
		std::shared_ptr<RHIWindow> window_system;
	};

	

	class RHI
	{
	public:
		virtual ~RHI() = 0;
		virtual void initialize(RHIInitInfo& initialize_info) = 0;
		virtual void createSwapchain() = 0;
		virtual void createCommandPool() = 0;
		virtual bool createRenderPass(const RHIRenderPassCreateInfo* pCreateInfo, RHIRenderPass*& pRenderPass) = 0;
		virtual bool createGraphicsPipeline(const RHIPipelineCreateInfo* pCreateInfo, RHIPipeline*& pPipeline) = 0;
		virtual bool createFrameBuffer(const RHIFramebufferCreateInfo* pCreateInfo, RHIFramebuffer*& pPipeline) = 0;
		//virtual void UpdateDraw(float dt) = 0;
		virtual bool prepareDraw(float dt, RHIDrawInfo& drawinfo) = 0;
		virtual void UpdateDraw(float dt, RHIDrawInfo& drawinfo) = 0;
		virtual void submitDraw(float dt, RHIDrawInfo& drawinfo) = 0;
		virtual void waitFrameTime(float wait_deltaTime) = 0;
		virtual RHICommandBuffer* getCommandBuffer() = 0;
		virtual void recreateSwapchain() = 0;
		virtual bool AllocateDescriptorSets(const RHIDescriptorSetAllocateInfo* pAllocateInfo, RHIDescriptorSet*& pDescriptorSets) = 0;
		virtual bool updateDescriptorSets(RHIUpdatesDescriptorSetsInfo& writeinfo) = 0;


		virtual void createVertexBuffer(void* data, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory) = 0;
		virtual void createIndexBuffer(void* data, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory) = 0;
		virtual void createUniformBuffer(void* mapdata, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory) = 0;
		

		virtual bool createDescriptorSetLayout(const RHIDescriptorSetLayoutCreateInfo* pCreateInfo, RHIDescriptorSetLayout*& pSetLayout) = 0;


		virtual void DrawMesh(RHIDrawInfo& draw_info, RHIDrawMeshInfo& draw_mesh_info) = 0;
		virtual void DrawDebug() = 0;
	public:
		const TBAlignedArray<RHIImageView*> getSwapChainImageViews() { return m_swapChainImageViews; }
		virtual FORCE_INLINE int getCurrentFrameIndex() = 0;
		virtual FORCE_INLINE int getMaxFrameIndex() = 0;
		FORCE_INLINE uint32_t getCurrentSwapchainImageIndex() { return m_current_swapchain_image_index; }

		CEvent* drawEvents;
	protected:
		TBAlignedArray<RHIImageView*> m_swapChainImageViews;
		uint32_t m_current_swapchain_image_index;

	};
	inline RHI::~RHI() = default;

	class RHI_Factory :public Singleton<RHI_Factory>
	{
	public:
		static std::shared_ptr<RHI> createRHI(const RHI_TYPE& type);
		static std::shared_ptr<RHIWindow> createRHIWindow(const RHI_TYPE& type);
	};
}