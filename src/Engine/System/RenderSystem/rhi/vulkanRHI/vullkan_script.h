#pragma once


#include "rhi/rhi_script.h"
#include "common.hpp"
#include "ECSCommon.h"
#include "Structure/TB_AlignedArray.hpp"
#include "vullkan_rhi_resource.h"
#include "vullkan_rhi.h"
#include "vullkan_renderpass.h"


namespace TourBillon
{
   
    class VulkanScript : public RHIScript
    {
    public:
        ~VulkanScript();
        virtual void initialize(RHIScriptInfo& initialize_info) override;

    private:
        
        VkFramebuffer m_framebuffer{ nullptr };
        
    };
}