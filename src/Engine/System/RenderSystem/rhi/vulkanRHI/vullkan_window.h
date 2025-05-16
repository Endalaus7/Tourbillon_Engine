#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "rhi/rhi_window.h"
#include <vector>
#include "common.hpp"
#include "ECSCommon.h"
#include <optional>
//#include "Structure/TB_AlignedArray.hpp"
#include "vullkan_rhi_resource.h"
#include "vullkan_rhi.h"
#include "vullkan_port.h"
#include "vullkan_renderpass.h"

//同窗口下只使用同一种Renderpass，减小开销

namespace TourBillon
{
    struct RHIWindowInfo;
    
    class Buttons;
    class Mouse;

    class VulkanWindow final : public RHIWindow
    {
        friend class VulkanPort;
    public:
        ~VulkanWindow();
        virtual void initialize(RHIWindowInfo& initialize_info) override final;
        virtual const RHIWindowInfo& getWindowInfo()override;
        virtual bool shouldClose()override;
        virtual void pollEvents();

        virtual void update(float dt)override;

        GLFWwindow* getWindow();

        bool updateWindow();


        struct SwapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        struct VulkanFramebufferCreateInfo {
            //uint32_t windowIndex;
            uint32_t width;
            uint32_t height;
            VulkanRenderPass* renderpass;
            TBAlignedArray<VkImageView> imageviews;
        };

        void createWindowSurface();
        void createSwapchain();
        void createSwapchainImageViews();
        void createFrameBufferImageAndView();
        void createCommandBuffers(VkDevice device);
        void setCommandBuffers(const std::array<VkCommandBuffer, s_max_frames_in_flight>& srcbuffer);
        bool createSemaphore();
        void createFrameBuffer(const VulkanFramebufferCreateInfo* pCreateInfo);


        virtual void updateBuffer(void* data, RHIBuffer* buffer, RHIDeviceSize totaloffset, RHIDeviceSize totalsize);

        void cleanup();
    protected:

        virtual void recreateSwapchain();
        void clearSwapchain();
        void bindWindowCallback();

        static void static_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (s_window_instance)
                s_window_instance->keyCallback(window, key, scancode, action, mods);
        }
        void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void static_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
        {
            if (s_window_instance)
                s_window_instance->mouseButtonCallback(window, button, action, mods);
        }
        void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void static_cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
        {
            if (s_window_instance)
                s_window_instance->cursorPosCallback(window, xpos, ypos);
        }
        void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

        
    private:
        struct QueueFamilyIndices {
            std::optional<uint32_t> graphicsFamily;//图像队列
            std::optional<uint32_t> presentFamily;//呈现队列
            bool isComplete() {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    private:
        static VulkanWindow* s_window_instance;
        //std::shared_ptr<VulkanRHI> m_vkrhi;

        GLFWwindow* m_window{ nullptr };

        //VkDevice m_device{ nullptr };
        

        VkSurfaceKHR m_surface;
        //VkCommandPool m_commandPools;

        VkSwapchainKHR m_swapChain;
        TBAlignedArray<VkImage> m_swapChainImages;
        TBAlignedArray<VkImageView> m_swapChainImageViews;
        TBAlignedArray<VkFramebuffer>   m_framebuffer;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_wapChainExtent;

        std::array<VkCommandBuffer, s_max_frames_in_flight> m_commandBuffers;//同场景同shader可共用

        std::array<VkSemaphore, s_max_frames_in_flight> m_imageAvailableSemaphore;
        std::array<VkSemaphore, s_max_frames_in_flight> m_renderFinishedSemaphore;
        std::array<VkFence, s_max_frames_in_flight>     m_inFlightFence;


        uint8_t              m_current_frame_index{ 0 };

        static bool m_islock;//锁定鼠标
        static int m_lockx, m_locky;//锁定位置
        static double m_lastx, m_lasty;//上一帧位置
    };
}