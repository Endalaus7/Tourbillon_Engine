#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "rhi/rhi.h"

#include <vector>
#include <optional>
#include "vullkan_rhi_resource.h"

namespace TourBillon
{
    class VulkanWindow;


    class VulkanRHI final : public RHI
    {
    public:
        ~VulkanRHI()override;
        virtual void initialize(RHIInitInfo& initialize_info) override;

        //virtual void UpdateDraw(float dt);
        virtual bool prepareDraw(float dt, RHIDrawInfo& drawinfo)override;		
        virtual void submitDraw(float dt, RHIDrawInfo& drawinfo)override;
        virtual void UpdateDraw(float dt, RHIDrawInfo& drawinfo)override;

        virtual void waitFrameTime(float wait_deltaTime);

        virtual RHICommandBuffer* getCommandBuffer()override;

        virtual FORCE_INLINE int getCurrentFrameIndex()override { return m_current_frame_index; }
        virtual FORCE_INLINE int getMaxFrameIndex()override { return s_max_frames_in_flight; }

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
        void createSwapchain() override;
        bool createRenderPass(const RHIRenderPassCreateInfo* pCreateInfo, RHIRenderPass*& pRenderPass)override;
        bool createDescriptorSetLayout(const RHIDescriptorSetLayoutCreateInfo* pCreateInfo, RHIDescriptorSetLayout*& pSetLayout) override;
        bool AllocateDescriptorSets(const RHIDescriptorSetAllocateInfo* pAllocateInfo, RHIDescriptorSet*& pDescriptorSets) override;
        bool updateDescriptorSets(RHIUpdatesDescriptorSetsInfo& writeinfo) override;
        void createSwapchainImageViews();
        bool createGraphicsPipeline(const RHIPipelineCreateInfo* pCreateInfo, RHIPipeline*& pPipeline)override;
        bool createFrameBuffer(const RHIFramebufferCreateInfo* pCreateInfo, RHIFramebuffer*& pframeBuffer)override;
        void createFrameBufferImageAndView();
        bool createSemaphore();

        VkShaderModule createShaderModule(const std::vector<char>& code);

        virtual void updateBuffer(void* data, RHIBuffer* buffer, RHIDeviceSize offset, RHIDeviceSize size)override;

        virtual void createVertexBuffer(void* srcdata, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory)override;
        virtual void createIndexBuffer(void* srcdata, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory)override;
        virtual void createUniformBuffer(void* mapdata, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory)override;

        //void recordCommandBuffer(VkCommandBuffer commandBuffer, RHIDrawInfo* drawinfo);
        
        virtual void recreateSwapchain();
        void transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
        
        bool checkValidationLayerSupport();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        bool isDeviceSuitable(VkPhysicalDevice device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device); 
        std::vector<const char*> getRequiredExtensions();
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties); 
        

        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        

        RHICommandBuffer* beginSingleTimeCommands();
        void            endSingleTimeCommands(RHICommandBuffer* command_buffer);

        void cleanup();
        virtual void clearSwapchain();


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

        VulkanWindow* m_vkWindow;//后续可以改多个window
        //GLFWwindow* m_window{ nullptr };
        VkInstance m_instance{ nullptr };
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device{ nullptr };
        VkQueue m_graphicsQueue;
        VkQueue m_presentQueue;
        VkSwapchainKHR m_swapChain{ nullptr };
        VkSurfaceKHR       m_surface{ nullptr };
        TBAlignedArray<VkImage> m_swapChainImages;
        
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_wapChainExtent;
        VkRenderPass m_renderPass;
        VkPipeline m_graphicsPipeline;
        VkCommandPool m_commandPool;
        VulkanCommandBuffer m_commandBuffers[s_max_frames_in_flight];
        VkDebugUtilsMessengerEXT m_debugMessenger;
        TBAlignedArray<VkFramebuffer> m_swapChainFramebuffers;
        VkDescriptorPool m_descriptorPool;

        VkSemaphore m_imageAvailableSemaphore[s_max_frames_in_flight];
        VkSemaphore m_renderFinishedSemaphore[s_max_frames_in_flight];
        VkFence m_inFlightFence[s_max_frames_in_flight];
    };
}