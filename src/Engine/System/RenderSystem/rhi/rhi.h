#pragma once

#include <memory>
#include "common.hpp"
#include "log.hpp"
#include "rhi_struct.h"
#include "rhi_source.h"
#include "Structure/TB_AlignedArray.hpp"
#include "Structure/TB_List.hpp"


//rhi的instance

namespace TourBillon
{
	class RHIWindow;

	enum RHI_TYPE
	{
		VULKAN_RHI = 0x0
	};

	struct RHIInitInfo
	{
		//TBVector<std::shared_ptr<RHIWindow>> window_systems;
	};

	

	class RHI
	{
	public:
		virtual ~RHI() = 0;
		virtual void initialize(RHIInitInfo& initialize_info) = 0;
		//virtual void createSwapchain(uint32_t index) = 0;
		virtual void createCommandPool() = 0;
		//virtual bool createRenderPass(const RHIRenderPassCreateInfo* pCreateInfo, RHIRenderPass*& pRenderPass) = 0;
		//virtual bool createGraphicsPipeline(const RHIPipelineCreateInfo* pCreateInfo, RHIPipeline*& pPipeline) = 0;
		//virtual bool createFrameBuffer(const RHIFramebufferCreateInfo* pCreateInfo, RHIFramebuffer*& pPipeline) = 0;
		//virtual void UpdateDraw(float dt) = 0;

		virtual void BeforeFrameDraw(float dt) = 0;
		virtual bool prepareDraw(float dt, RHIDrawInfo& drawinfo) = 0;
		virtual void UpdateDraw(float dt, RHIDrawInfo& drawinfo) = 0;
		virtual void submitDraw(float dt, RHIDrawInfo& drawinfo) = 0;
		virtual void AfterFrameDraw(float dt) = 0;

		virtual void waitFrameTime(float wait_deltaTime) = 0;
		//virtual RHICommandBuffer* getCommandBuffer(uint32_t windowindex) = 0;
		virtual void recreateSwapchain() = 0;
		virtual void destroyFramebuffer(RHIFramebuffer* framebuffer) = 0;
		virtual bool AllocateDescriptorSets(const RHIDescriptorSetAllocateInfo* pAllocateInfo, RHIDescriptorSet*& pDescriptorSets) = 0;
		virtual bool updateDescriptorSets(RHIUpdatesDescriptorSetsInfo& writeinfo) = 0;

		virtual void createTextureImage(void* imgdata, size_t imageSize, int imageWidth, int imageHeight, int texChannels, RHIImage*& image_buffer, RHIDeviceMemory*& buffer_memory) = 0;
		virtual void createTextureSampler(const RHICreateTextureSamplerInfo& createinfo, RHISampler*& sampler) = 0;

		//virtual void updateBuffer(void* data, uint32_t windowindex, RHIBuffer* buffer, RHIDeviceSize offset, RHIDeviceSize size) = 0;

		virtual void createVertexBuffer(void* data, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory) = 0;
		virtual void createIndexBuffer(void* data, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory) = 0;
		virtual void createUniformBuffer(void* mapdata, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory) = 0;
		
		virtual void destroyImage(RHIImage*& image) = 0;
		virtual void destroyBuffer(RHIBuffer*& buffer) = 0;
		virtual void destroyMemory(RHIDeviceMemory*& memory) = 0;
		virtual void destroySampler(RHISampler*& sampler) = 0;

		virtual bool createDescriptorSetLayout(const RHIDescriptorSetLayoutCreateInfo* pCreateInfo, RHIDescriptorSetLayout*& pSetLayout) = 0;

		virtual void DrawViewport(RHIDrawInfo& draw_info) = 0;
		virtual void DrawMesh(RHIDrawInfo& draw_info, RHIDrawMeshInfo& draw_mesh_info) = 0;
		virtual void DrawDebug() = 0;
	public:
		//const TBAlignedArray<RHIImageView*> getSwapChainImageViews(uint32_t index) { return m_swapChainImageViews[index]; }
		//virtual FORCE_INLINE int getCurrentFrameIndex() = 0;
		virtual FORCE_INLINE uint8_t getStaticMaxFramesInFlight() = 0;
		//FORCE_INLINE uint32_t getCurrentSwapchainImageIndex(uint32_t index) { return m_current_swapchain_image_index[index]; }
		//FORCE_INLINE uint32_t getWindowNum(uint32_t index) { return m_windowSize; }

		CEvent* drawEvents;
		//uint32_t m_windowSize = 0;//总窗口个数,删除
	protected:
		//TBVector<TBAlignedArray<RHIImageView*>> m_swapChainImageViews;//第一个index是窗口，第二个是交换链
		//TBVector<uint32_t> m_current_swapchain_image_index;

	};
	inline RHI::~RHI() = default;

	class RHI_Factory :public Singleton<RHI_Factory>
	{
	public:
		static std::shared_ptr<RHI> getRHI(const RHI_TYPE& type);
		static std::shared_ptr<RHIWindow> createRHIWindow(const RHI_TYPE& type);
	private:
		static std::shared_ptr<RHI> m_RHIinstance; // 全局唯一实例
	};
}