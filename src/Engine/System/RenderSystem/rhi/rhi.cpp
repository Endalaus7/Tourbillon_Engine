#include "rhi.h"

#include "vulkanRHI/vullkan_rhi.h"
#include "vulkanRHI/vullkan_window.h"

std::shared_ptr<TourBillon::RHI> TourBillon::RHI_Factory::createRHI(const RHI_TYPE& type)
{
	switch (type)
	{
	case VULKAN_RHI:
		return std::make_shared<VulkanRHI>();
	default:
		break;
	}
	return std::make_shared<VulkanRHI>();
}

std::shared_ptr<TourBillon::RHIWindow> TourBillon::RHI_Factory::createRHIWindow(const RHI_TYPE& type)
{
	switch (type)
	{
	case VULKAN_RHI:
		return std::make_shared<VulkanWindow>();
	default:
		break;
	}
	return std::make_shared<VulkanWindow>();
}

void TourBillon::RHI::BeforeFrameDraw(float dt)
{
	m_Drawing = true;
}

void TourBillon::RHI::AfterFrameDraw(float dt)
{
	m_Drawing = false;
}
