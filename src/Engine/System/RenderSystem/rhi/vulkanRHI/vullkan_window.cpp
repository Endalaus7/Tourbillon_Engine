#include "vullkan_window.h"
#include "ECSManager.h"
#include "components/KeysComponent.h"
#include "components/WindowComponent.h"

bool TourBillon::VulkanWindow::m_islock = false;
int TourBillon::VulkanWindow::m_lockx = 0;
int TourBillon::VulkanWindow::m_locky = 0;
double TourBillon::VulkanWindow::m_lastx = 0;
double TourBillon::VulkanWindow::m_lasty = 0;
TourBillon::VulkanWindow::~VulkanWindow()
{
    
}

void TourBillon::VulkanWindow::initialize(RHIWindowInfo& initialize_info)
{
    RHIWindow::initialize(initialize_info);
    if (!glfwInit())
    {
        //LOG(__FUNCTION__, "failed to initialize GLFW");
        return;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //auto sharewindow = std::dynamic_pointer_cast<VulkanWindow>(initialize_info.sharedwindow);
    //m_window = glfwCreateWindow(initialize_info.width, initialize_info.height, initialize_info.title, nullptr, sharewindow ? sharewindow->getWindow() : nullptr);
    if (!m_window)
    {
        //LOG_FATAL(__FUNCTION__, "failed to create window");
        glfwTerminate();
        return;
    }
    
    glfwMakeContextCurrent(m_window);
    bindWindowCallback();

    createWindowSurface();

}
void TourBillon::VulkanWindow::bindWindowCallback()
{
    // Setup input callbacks
   //glfwSetWindowUserPointer(m_window, this);//后续写一个vulkan_window类型，承载此处的回调
    glfwSetKeyCallback(m_window, static_keyCallback);
    //glfwSetCharCallback(m_window, charCallback);
    //glfwSetCharModsCallback(m_window, charModsCallback);
    glfwSetMouseButtonCallback(m_window, static_mouseButtonCallback);
    glfwSetCursorPosCallback(m_window, static_cursorPosCallback);
    //glfwSetCursorEnterCallback(m_window, cursorEnterCallback);
    //glfwSetScrollCallback(m_window, scrollCallback);
    //glfwSetDropCallback(m_window, dropCallback);
    //glfwSetWindowSizeCallback(m_window, windowSizeCallback);
    //glfwSetWindowCloseCallback(m_window, windowCloseCallback);

   // glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);

}
const TourBillon::RHIWindowInfo& TourBillon::VulkanWindow::getWindowInfo()
{
    // TODO: 在此处插入 return 语句
    return m_windowInfo;
}


bool TourBillon::VulkanWindow::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}

void TourBillon::VulkanWindow::pollEvents()
{
    glfwPollEvents();
}

void TourBillon::VulkanWindow::update(float dt)
{

}

GLFWwindow* TourBillon::VulkanWindow::getWindow()
{
	return m_window;
}

bool TourBillon::VulkanWindow::updateWindow()
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);
    while (width == 0 || height == 0) // minimized 0,0, pause for now
    {
        glfwGetFramebufferSize(m_window, &width, &height);
        glfwWaitEvents();
    }

    if (width == m_width && height == m_height)
        return false;

    m_width = width;
    m_height = height;

    return true;
}

void TourBillon::VulkanWindow::createWindowSurface()
{
    std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();
    if (glfwCreateWindowSurface(vkrhi->getInterface(), m_window, nullptr, &m_surface) != VK_SUCCESS)
    {
        LOG_WARNING("failed to create window surface!");
    }
    vkrhi->registerSurface(m_surface);//创建好的surface需要写回rhi，用于创建可共用的逻辑设备等
    
}

void TourBillon::VulkanWindow::createSwapchain()
{
    std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(vkrhi->getPhysicalDevice());

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(vkrhi->getPhysicalDevice());
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VkDevice logic_device = vkrhi->getDevice();

    if (vkCreateSwapchainKHR(logic_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
        LOG_ERROR("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(logic_device, m_swapChain, &imageCount, nullptr);
    m_swapChainImages.apply(imageCount);
    vkGetSwapchainImagesKHR(logic_device, m_swapChain, &imageCount, m_swapChainImages.data());

    m_swapChainImageFormat = surfaceFormat.format;
    m_wapChainExtent = extent;

}

void TourBillon::VulkanWindow::createSwapchainImageViews()
{
    std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();

    m_swapChainImageViews.apply(m_swapChainImages.size());

    for (size_t i = 0; i < m_swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        //VulkanImageView* swapChainImageViews = new VulkanImageView;

        if (vkCreateImageView(vkrhi->getDevice(), &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS) {
            LOG_WARNING("failed to create image views!");
        }
        //m_swapChainImageViews[i] = swapChainImageViews;
    }
    return;
}
        
void TourBillon::VulkanWindow::createFrameBufferImageAndView()
{
    std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();

    VulkanCommandBuffer* vk_commandBuffer = (VulkanCommandBuffer*)vkrhi->beginSingleTimeCommands();
    for (int frame = 0; frame < s_max_frames_in_flight; frame++)
        vkrhi->transitionImageLayout(vk_commandBuffer->commandbuffer, m_swapChainImages[frame], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    vkrhi->endSingleTimeCommands(vk_commandBuffer);
}

void TourBillon::VulkanWindow::createCommandBuffers(VkDevice device)
{
    VkCommandBufferAllocateInfo allocInfo{};

    //VkCommandBuffer tmp_commandbuffer[s_max_frames_in_flight];
    std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();

    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vkrhi->getCommandPool();// m_commandPools;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = s_max_frames_in_flight;

    if (vkAllocateCommandBuffers(device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
        LOG_WARNING("failed to allocate command buffers!");
    }
}
void TourBillon::VulkanWindow::setCommandBuffers(const std::array<VkCommandBuffer, s_max_frames_in_flight>& srcbuffer)
{
    std::copy(srcbuffer.begin(), srcbuffer.end(), m_commandBuffers.begin());
}
bool TourBillon::VulkanWindow::createSemaphore()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();

    VkDevice logic_device = vkrhi->getDevice();

    for (int i = 0; i < s_max_frames_in_flight; i++)
    {
        if (vkCreateSemaphore(logic_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore[i]) != VK_SUCCESS ||
            vkCreateSemaphore(logic_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore[i]) != VK_SUCCESS ||
            vkCreateFence(logic_device, &fenceInfo, nullptr, &m_inFlightFence[i]) != VK_SUCCESS) {
            LOG_WARNING("failed to create synchronization objects for a frame!");
            return false;
        };
    }
    return true;
}
void TourBillon::VulkanWindow::createFrameBuffer(const VulkanFramebufferCreateInfo* pCreateInfo)
{
    m_framebuffer.resize(m_swapChainImageViews.size());
    for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
    {
        //attachments根据renderpass生成
        TBAlignedArray<VkImageView> attachments(pCreateInfo->imageviews.size());
        for (int i = 0; i < pCreateInfo->imageviews.size(); i++)
        {
            attachments[i] = pCreateInfo->imageviews[i];// m_swapChainImageViews[i];depthImageView
        }

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = pCreateInfo->renderpass->getRenderPass();
        framebufferInfo.attachmentCount = attachments.size();
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = pCreateInfo->width;
        framebufferInfo.height = pCreateInfo->height;
        framebufferInfo.layers = 1;

        std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();

        if (vkCreateFramebuffer(vkrhi->getDevice(), &framebufferInfo, nullptr, &m_framebuffer[i]) != VK_SUCCESS) {
            LOG_WARNING("failed to create framebuffer!");
        }
    }

}
void TourBillon::VulkanWindow::updateBuffer(void* data, RHIBuffer* buffer, RHIDeviceSize totaloffset, RHIDeviceSize totalsize)
{
    VkCommandBuffer commandBuffer = m_commandBuffers[m_current_frame_index];
    VulkanBuffer* vk_buffer = dynamic_cast<VulkanBuffer*>(buffer);
    size_t chunkSize = 65536;
    for (size_t ioffset = 0; ioffset < totalsize - totaloffset; ioffset += chunkSize) {
        size_t isize = min(chunkSize, totalsize - totaloffset - ioffset);
        vkCmdUpdateBuffer(commandBuffer, vk_buffer->buffer, ioffset, isize, data);
    }
}
void TourBillon::VulkanWindow::cleanup()
{
    std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();

    VkDevice logic_device = vkrhi->getDevice();

    for (size_t i = 0; i < s_max_frames_in_flight; i++) {
        vkDestroySemaphore(logic_device, m_renderFinishedSemaphore[i], nullptr);
        vkDestroySemaphore(logic_device, m_imageAvailableSemaphore[i], nullptr);
        vkDestroyFence(logic_device, m_inFlightFence[i], nullptr);
    }
    clearSwapchain();
    glfwDestroyWindow(m_window);
}
void TourBillon::VulkanWindow::recreateSwapchain()
{
    bool needrecreate = updateWindow();
    std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();

    if (needrecreate)
    {
        vkDeviceWaitIdle(vkrhi->getDevice());

        //vkDestroyImageView();
        clearSwapchain();

        createSwapchain();
        createSwapchainImageViews();
        createFrameBufferImageAndView();
    }
}

void TourBillon::VulkanWindow::clearSwapchain()
{
    std::shared_ptr<VulkanRHI> vkrhi = GET_VULKAN_RHI();

    VkDevice logic_device = vkrhi->getDevice();
    for (auto imageview : m_swapChainImageViews)
    {
        vkDestroyImageView(logic_device, imageview, NULL);
    }
    if (m_swapChain)
        vkDestroySwapchainKHR(logic_device, m_swapChain, NULL); // also swapchain images
}

TourBillon::VulkanWindow::SwapChainSupportDetails TourBillon::VulkanWindow::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR TourBillon::VulkanWindow::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkExtent2D TourBillon::VulkanWindow::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }
    else {
        int width, height;
        //glfwGetFramebufferSize(m_vkWindows[index]->getWindow(), &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

VkPresentModeKHR TourBillon::VulkanWindow::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

TourBillon::VulkanWindow::QueueFamilyIndices TourBillon::VulkanWindow::findQueueFamilies(VkPhysicalDevice device)
{

    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        //VkBool32 presentSupport = false;

        VkBool32 presentSupport = true;

        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);
        if (presentSupport == false)
            break;

        //vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface[index], &presentSupport);

        //if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }
        i++;
    }
    return indices;
}





void TourBillon::VulkanWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto& buttons = ECSManager::Instance()->GetComponent<Buttons>(m_window_entity);
    buttons.baseWindow = m_window_entity;
    if(action == GLFW_PRESS)
        buttons.keyvalue.set(key);
    else if(action == GLFW_RELEASE)
        buttons.keyvalue.reset(key);
    ECSManager::Instance()->SendEvent(Events::KEY_CHANGE, (void*)m_window_entity);
}



void TourBillon::VulkanWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    auto& mouse = ECSManager::Instance()->GetComponent<Mouse>(m_window_entity);
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action)
        {
            if (!m_islock)
            {
                m_lockx = m_lastx;
                m_locky = m_lasty;
                m_islock = true;
            }
            glfwSetCursorPos(window, m_lockx, m_locky);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouse.pressRight = true;
        }
        else if (action == GLFW_RELEASE)
        {
            glfwSetCursorPos(window, m_lockx, m_locky);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_islock = false;
            mouse.pressRight = false;
        }

    }

}

void TourBillon::VulkanWindow::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto& mouse = ECSManager::Instance()->GetComponent<Mouse>(m_window_entity);
    mouse.baseWindow = m_window_entity;
    mouse.mousePos = TBMath::Vec2(xpos, ypos);
    mouse.mouseOffset = TBMath::Vec2(xpos - m_lastx, ypos - m_lasty);
    
    //LOG_DEBUG(std::to_string(mouse.mouseOffset.x) + "," + std::to_string(mouse.mouseOffset.y));

    ECSManager::Instance()->SendEvent(Events::MOUSE_MOVE, (void*)m_window_entity);

    m_lastx = xpos;
    m_lasty = ypos;
}

