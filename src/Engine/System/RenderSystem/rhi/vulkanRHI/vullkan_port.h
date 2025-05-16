#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
//#include "rhi/rhi_port.h"
#include <vector>
#include <array>
#include "common.hpp"
#include "ECSCommon.h"
#include "Structure/TB_AlignedArray.hpp"
#include "vullkan_rhi_resource.h"
#include "vullkan_rhi.h"

//渲染在一个视口上的实现
//每个port只属于一个窗口


namespace TourBillon
{
    struct RHIPortInfo
    {

    };
    class VulkanWindow;
    class VulkanPort// final : public RHIRenderPort
    {
    public:
        ~VulkanPort();
        virtual void initialize(RHIPortInfo& initialize_info);
        //virtual bool shouldClose()override;
        //virtual void pollEvents();
       
    private:
        VulkanWindow* pwindow{ nullptr };
        //std::shared_ptr<VulkanRHI> m_vkrhi;

        VkViewport viewport;
        VkRect2D rect;
        
    };
}