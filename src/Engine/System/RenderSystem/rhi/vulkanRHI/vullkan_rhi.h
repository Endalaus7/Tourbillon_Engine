#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "rhi/rhi.h"

#include <vector>
#include <array>
#include <optional>
#include "vullkan_rhi_resource.h"

//rhi是唯一的，因此不应该存储window、Swapchain、Semaphore、Fence的成员信息，而是存到rhiwindow中


namespace TourBillon
{
    class VulkanWindow;


    class VulkanRHI final : public RHI
    {
    public:
        ~VulkanRHI()override;
        virtual void initialize(RHIInitInfo& initialize_info) override;

        //virtual void UpdateDraw(float dt);
        virtual void BeforeFrameDraw(float dt)override;
        virtual bool prepareDraw(float dt, RHIDrawInfo& drawinfo)override;		
        virtual void submitDraw(float dt, RHIDrawInfo& drawinfo)override;
        virtual void UpdateDraw(float dt, RHIDrawInfo& drawinfo)override;
        virtual void AfterFrameDraw(float dt)override;

        virtual void createTextureImage(void* imgdata, size_t imageSize,int imageWidth, int imageHeight, int texChannels, RHIImage*& image_buffer, RHIDeviceMemory*& buffer_memory)override;
        virtual void createTextureSampler(const RHICreateTextureSamplerInfo& createinfo, RHISampler*& sampler)override;

        void multiWindowResize(uint32_t windowsize);

        virtual void waitFrameTime(float wait_deltaTime);

        virtual RHICommandBuffer* getCommandBuffer(uint32_t windowindex)override;

        virtual FORCE_INLINE int getCurrentFrameIndex()override { return m_current_frame_index; }
        virtual FORCE_INLINE int getMaxFrameIndex()override { return s_max_frames_in_flight; }

        virtual void DrawViewport(RHIDrawInfo& draw_info)override;
        virtual void DrawMesh(RHIDrawInfo& draw_info, RHIDrawMeshInfo& draw_mesh_info)override;
        virtual void DrawDebug()override;

    private:
        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };
        struct QueueFamilyIndices {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool isComplete() {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };
        const std::vector<const char*> m_deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_EXT_ROBUSTNESS_2_EXTENSION_NAME
        };

        void createInstance();
        void initializeDebugMessenger();
        void createWindowSurface();
        void initializePhysicalDevice();
        void createLogicalDevice();
        void createCommandPool() override;
        void createCommandBuffers();
        void createDescriptorPool();
        void createSyncPrimitives();
        void createAssetAllocator();
        void createSwapchain(uint32_t index) override;
        bool createRenderPass(const RHIRenderPassCreateInfo* pCreateInfo, RHIRenderPass*& pRenderPass)override;
        bool createDescriptorSetLayout(const RHIDescriptorSetLayoutCreateInfo* pCreateInfo, RHIDescriptorSetLayout*& pSetLayout) override;
        bool AllocateDescriptorSets(const RHIDescriptorSetAllocateInfo* pAllocateInfo, RHIDescriptorSet*& pDescriptorSets) override;
        bool updateDescriptorSets(RHIUpdatesDescriptorSetsInfo& writeinfo) override;
        void createSwapchainImageViews(uint32_t index);
        bool createGraphicsPipeline(const RHIPipelineCreateInfo* pCreateInfo, RHIPipeline*& pPipeline)override;
        bool createFrameBuffer(const RHIFramebufferCreateInfo* pCreateInfo, RHIFramebuffer*& pframeBuffer)override;
        void createFrameBufferImageAndView(uint32_t index);
        
        bool createSemaphore();

        VkShaderModule createShaderModule(const std::vector<char>& code);

        virtual void updateBuffer(void* data, uint32_t windowindex, RHIBuffer* buffer, RHIDeviceSize offset, RHIDeviceSize size)override;

        virtual void createVertexBuffer(void* srcdata, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory)override;
        virtual void createIndexBuffer(void* srcdata, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory)override;
        virtual void createUniformBuffer(void* mapdata, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory)override;

        //void recordCommandBuffer(VkCommandBuffer commandBuffer, RHIDrawInfo* drawinfo);
        
        virtual void recreateSwapchain();
        void transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
        
        bool checkValidationLayerSupport();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        bool isDeviceSuitable(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, uint32_t index);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t index);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device); 
        std::vector<const char*> getRequiredExtensions();
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties); 
        
        void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        VkImageView createImageView(VkImage image, VkFormat format); 

        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        

        RHICommandBuffer* beginSingleTimeCommands(uint32_t windowindex);
        void            endSingleTimeCommands(RHICommandBuffer* command_buffer, uint32_t windowindex);

		virtual void destroyImage(RHIImage*& image)override;
		virtual void destroyBuffer(RHIBuffer*& buffer)override;
		virtual void destroyMemory(RHIDeviceMemory*& memory)override;
        virtual void destroySampler(RHISampler*& sampler)override;

        void cleanup();
        virtual void clearSwapchain(uint32_t windowindex);
        virtual void destroyFramebuffer(RHIFramebuffer* framebuffer)override;

    protected:
        void createBuffer(RHIDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    private:
        

        bool m_enable_validation_Layers{ true };

        const std::vector<char const*> m_validation_layers{ "VK_LAYER_KHRONOS_validation" };
        uint32_t                       m_vulkan_api_version{ VK_API_VERSION_1_0 };

        // command pool and buffers
        uint32_t m_current_swapchain_image_index;
        uint8_t              m_current_frame_index{ 0 };
        static uint8_t const s_max_frames_in_flight{ 3 };

        TBVector<VulkanWindow*> m_vkWindows;//后续可以改多个window
        //GLFWwindow* m_window{ nullptr };
        VkInstance m_instance{ nullptr };
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device{ nullptr };
        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;
        TBVector<VkSwapchainKHR> m_swapChain;
        TBVector<VkSurfaceKHR>       m_surfaces;
        TBVector<TBAlignedArray<VkImage>> m_swapChainImages;
        
        VkFormat m_swapChainImageFormat;//暂时统一
        TBVector<VkExtent2D> m_wapChainExtent;
        VkRenderPass m_renderPass;
        VkPipeline m_graphicsPipeline;
        TBVector<VkCommandPool> m_commandPools;
        TBVector<std::array<VulkanCommandBuffer, s_max_frames_in_flight>> m_commandBuffers;
        VkDebugUtilsMessengerEXT m_debugMessenger;
        VkDescriptorPool m_descriptorPool;

        TBVector<std::array<VkSemaphore, s_max_frames_in_flight>> m_imageAvailableSemaphore;
        TBVector<std::array<VkSemaphore, s_max_frames_in_flight>> m_renderFinishedSemaphore;
        TBVector<std::array<VkFence, s_max_frames_in_flight>>     m_inFlightFence;
    };
}