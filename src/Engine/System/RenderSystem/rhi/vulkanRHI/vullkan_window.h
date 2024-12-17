#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "rhi/rhi_window.h"
#include <vector>
#include "common.hpp"

namespace TourBillon
{
    struct WindowCreateInfo;
    

    class VulkanWindow final : public RHIWindow
    {
    public:
        virtual void initialize(WindowCreateInfo& initialize_info) override final;
        virtual const WindowCreateInfo& getWindowInfo()override;
        virtual bool shouldClose()override;
        virtual void pollEvents();

        GLFWwindow* getWindow();

        bool updateWindow();

        FORCE_INLINE int getwidth() { return m_width; }
        FORCE_INLINE int getheight() { return m_height; }
    private:
        GLFWwindow* m_window{ nullptr };

        int m_width = 0;
        int m_height = 0;
    };
}