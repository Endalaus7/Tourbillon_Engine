#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "rhi/rhi_window.h"
#include <vector>
#include "common.hpp"
#include "ECSCommon.h"


namespace TourBillon
{
    struct WindowCreateInfo;
    
    class Buttons;
    class Mouse;

    class VulkanWindow final : public RHIWindow
    {
    public:
        ~VulkanWindow();
        virtual void initialize(WindowCreateInfo& initialize_info) override final;
        virtual const WindowCreateInfo& getWindowInfo()override;
        virtual bool shouldClose()override;
        virtual void pollEvents();

        GLFWwindow* getWindow();

        bool updateWindow();

       

    protected:
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    private:

        GLFWwindow* m_window{ nullptr };

        
        static bool m_islock;//锁定鼠标
        static int m_lockx, m_locky;//锁定位置
        static double m_lastx, m_lasty;//上一帧位置
    };
}