#include "rhi.h"

#include "vulkanRHI/vullkan_rhi.h"
#include "vulkanRHI/vullkan_window.h"

std::shared_ptr<TourBillon::RHI> TourBillon::RHI_Factory::m_RHIinstance = nullptr;

std::shared_ptr<TourBillon::RHI> TourBillon::RHI_Factory::getRHI(const RHI_TYPE& type)
{
	static std::once_flag flag;
	std::call_once(flag, [&]() {
		switch (type)
		{
		case VULKAN_RHI:
			m_RHIinstance = std::make_shared<VulkanRHI>();
		default:
			LOG_ERROR("Unknown RHI");
		}
	});
	return m_RHIinstance;
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
