#include "vullkan_rhi.h"
#include "vullkan_window.h"
#include "vullkan_rhi_resource.h"

#include <algorithm>
#include <set>
#include "ReadFile.hpp"


VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    //std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    std::string debug_message = "validation layer: " + std::string(pCallbackData->pMessage);
    LOG_WARNING(debug_message);
    return VK_FALSE;
}

TourBillon::VulkanRHI::~VulkanRHI()
{
    cleanup();

}
void TourBillon::VulkanRHI::initialize(RHIInitInfo& init_info)
{
    m_vkWindows.resize(init_info.window_systems.size());
    for (int iwindow = 0; iwindow < init_info.window_systems.size(); iwindow++)
        m_vkWindows[iwindow] = dynamic_cast<VulkanWindow*>(init_info.window_systems[iwindow].get());

    multiWindowResize(m_vkWindows.size());

	//WindowCreateInfo windowInfo = init_info.window_system->getWindowInfo();
    //m_vkWindow = std::make_shared<VulkanWindow>();
    createInstance();

    initializeDebugMessenger();//not ready

    createWindowSurface();

    initializePhysicalDevice();

    createLogicalDevice();

    createDescriptorPool();

    createSyncPrimitives();//not ready

    
    for (int iwindow = 0; iwindow < m_vkWindows.size(); iwindow++)
    {
        createSwapchain(iwindow);
        createSwapchainImageViews(iwindow);
    }

    createCommandPool();

    createCommandBuffers();

    createSemaphore();

    createAssetAllocator();//not ready
}

void TourBillon::VulkanRHI::multiWindowResize(uint32_t windowsize)
{
    m_windowSize = windowsize;
    m_swapChainImageViews.resize(windowsize);
    m_wapChainExtent.resize(windowsize);
    m_swapChain.resize(windowsize, nullptr);
    m_swapChainImages.resize(windowsize);

    m_imageAvailableSemaphore.resize(windowsize);
    m_renderFinishedSemaphore.resize(windowsize);
    m_inFlightFence.resize(windowsize);

    m_commandBuffers.resize(windowsize);
    m_commandPools.resize(windowsize);
}

void TourBillon::VulkanRHI::recreateSwapchain()
{
    uint32_t index = 0;
    for(auto window: m_vkWindows)
    {
        bool needrecreate = window->updateWindow();

        if (needrecreate)
        {
            vkDeviceWaitIdle(m_device);

            //vkDestroyImageView();
            clearSwapchain(index);

            createSwapchain(index);
            createSwapchainImageViews(index);
            createFrameBufferImageAndView(index);
        }
        index++;
    }
}

void TourBillon::VulkanRHI::transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = 0;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    }
    else {
        LOG_ERROR("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(commandBuffer,
        sourceStage, destinationStage,
        0, 0, nullptr, 0, nullptr, 1, &barrier);
}

bool TourBillon::VulkanRHI::prepareDraw(float dt, RHIDrawInfo& drawinfo)
{
    VkResult res_wait_for_fences = vkWaitForFences(m_device, 1, &m_inFlightFence[drawinfo.windowIndex][m_current_frame_index], VK_FALSE, UINT64_MAX);
    if (res_wait_for_fences != VK_SUCCESS)
    {
        LOG_ERROR("vkWaitForFences failed");
    }

    uint32_t swapchain_image_index;
    VkResult acquire_image_result = vkAcquireNextImageKHR(m_device, m_swapChain[drawinfo.windowIndex], UINT64_MAX, m_imageAvailableSemaphore[drawinfo.windowIndex][m_current_frame_index], VK_NULL_HANDLE, &swapchain_image_index);

    uint32_t imageCount;
    vkGetSwapchainImagesKHR(m_device, m_swapChain[drawinfo.windowIndex], &imageCount, nullptr); // 获取交换链图像数量

    if (VK_ERROR_OUT_OF_DATE_KHR == acquire_image_result)
    {
        recreateSwapchain();
            drawinfo.preEvents.trigger();
        return true;
    }
    else if (VK_SUBOPTIMAL_KHR == acquire_image_result)
    {
        recreateSwapchain();
            drawinfo.preEvents.trigger();
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT };
        //VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore[drawinfo.windowIndex][m_current_frame_index] };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_imageAvailableSemaphore[drawinfo.windowIndex][m_current_frame_index];
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 0;
        submitInfo.pCommandBuffers = NULL;
        submitInfo.signalSemaphoreCount = 0;
        submitInfo.pSignalSemaphores = NULL;

        VkResult res_reset_fences = vkResetFences(m_device, 1, &m_inFlightFence[drawinfo.windowIndex][m_current_frame_index]);
        if (VK_SUCCESS != res_reset_fences)
        {
            LOG_WARNING("vkResetFences failed!");
            return false;
        }

        if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, m_inFlightFence[drawinfo.windowIndex][m_current_frame_index]) != VK_SUCCESS) {
            LOG_WARNING("failed to submit draw command buffer!");
            return false;
        }

        //m_current_frame_index = (m_current_frame_index + 1) % s_max_frames_in_flight;
        return true;
    }
    else
    {
        if(acquire_image_result != VK_SUCCESS)
        {
            LOG_WARNING("vkAcquireNextImageKHR failed!");
            return false;
        }
    }

    m_current_swapchain_image_index = swapchain_image_index;

    VkCommandBufferBeginInfo cmd_buffer_beginInfo{};
    cmd_buffer_beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_buffer_beginInfo.flags = 0;
    cmd_buffer_beginInfo.pInheritanceInfo = nullptr;



    VkResult res_reset_fences = vkResetFences(m_device, 1, &m_inFlightFence[drawinfo.windowIndex][m_current_frame_index]);
    if (VK_SUCCESS != res_reset_fences)
    {
        LOG_WARNING("_vkResetFences failed!");
        return false;
    }

    vkResetCommandBuffer(m_commandBuffers[drawinfo.windowIndex][m_current_frame_index].commandbuffer, /*VkCommandBufferResetFlagBits*/ 0);

    if (vkBeginCommandBuffer(m_commandBuffers[drawinfo.windowIndex][m_current_frame_index].commandbuffer, &cmd_buffer_beginInfo) != VK_SUCCESS) {
        LOG_WARNING("failed to begin recording command buffer!");
        return false;
    }

    VkImageMemoryBarrier imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcAccessMask = 0;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = m_swapChainImages[drawinfo.windowIndex][m_current_frame_index];
    imageMemoryBarrier.subresourceRange = {};
    imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
    imageMemoryBarrier.subresourceRange.levelCount = 1;
    imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
    imageMemoryBarrier.subresourceRange.layerCount = 1;
    
    
    // 插入图像内存屏障
    //vkCmdPipelineBarrier(
    //    m_commandBuffers[drawinfo.windowIndex][m_current_frame_index].commandbuffer,
    //    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    //    0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier
    //);

    return false;
}

void TourBillon::VulkanRHI::submitDraw(float dt, RHIDrawInfo& drawinfo)
{
    VkResult res_end_command_buffer = vkEndCommandBuffer(m_commandBuffers[drawinfo.windowIndex][m_current_frame_index].commandbuffer);
    if (VK_SUCCESS != res_end_command_buffer)
    {
        LOG_WARNING("vkEndCommandBuffer failed!");
        return;
    }

    VkSubmitInfo         submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore[drawinfo.windowIndex][m_current_frame_index] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = waitSemaphores;
    submit_info.pWaitDstStageMask = waitStages;

    VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore[drawinfo.windowIndex][m_current_frame_index] };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signalSemaphores;

    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_commandBuffers[drawinfo.windowIndex][m_current_frame_index].commandbuffer;



    VkResult res_queue_submit = vkQueueSubmit(m_graphicsQueue, 1, &submit_info, m_inFlightFence[drawinfo.windowIndex][m_current_frame_index]);
    if(res_queue_submit != VK_SUCCESS)
    {
        LOG_ERROR("vkQueueSubmit failed!");
        return;
    }

    // present swapchain
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &m_renderFinishedSemaphore[drawinfo.windowIndex][m_current_frame_index];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapChain[drawinfo.windowIndex];
    presentInfo.pImageIndices = &m_current_swapchain_image_index;

    VkResult present_result = vkQueuePresentKHR(m_presentQueue, &presentInfo);
    if (VK_ERROR_OUT_OF_DATE_KHR == present_result || VK_SUBOPTIMAL_KHR == present_result)
    {
        recreateSwapchain();
    }
    else
    {
        if (VK_SUCCESS != present_result)
        {
            LOG_WARNING("vkQueuePresentKHR failed!");
            return;
        }
    }
    //m_current_frame_index = (m_current_frame_index + 1) % s_max_frames_in_flight;
    return ;
}
void TourBillon::VulkanRHI::UpdateDraw(float dt, RHIDrawInfo& drawinfo)
{
    
    //recordCommandBuffer(m_commandBuffers[m_current_frame_index].commandbuffer, drawinfo);
    VkCommandBuffer commandBuffer = m_commandBuffers[drawinfo.windowIndex][m_current_frame_index].commandbuffer;
    VulkanRenderPass* vk_renderpass = dynamic_cast<VulkanRenderPass*>(drawinfo.renderpass);
    VulkanFramebuffer* vk_framebuffer = dynamic_cast<VulkanFramebuffer*>(drawinfo.framebuffers);
    VulkanPipeline* vk_pipeline = dynamic_cast<VulkanPipeline*>(drawinfo.pipeline);

    VkRenderPassBeginInfo vk_render_pass_begin_info{};
    vk_render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    vk_render_pass_begin_info.renderPass = vk_renderpass->renderpass;
    vk_render_pass_begin_info.framebuffer = vk_framebuffer->framebuffer;
    vk_render_pass_begin_info.renderArea.offset = { 0, 0 };
    vk_render_pass_begin_info.renderArea.extent = m_wapChainExtent[drawinfo.windowIndex];
    vk_render_pass_begin_info.renderArea.extent = m_wapChainExtent[drawinfo.windowIndex];
    VkClearValue clearColor = { {{0.1f, 0.1f, 0.1f, 1.0f}} };
    vk_render_pass_begin_info.clearValueCount = 1;
    vk_render_pass_begin_info.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &vk_render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipeline->pipeline);

        //drawinfo.drawEvents->event_data = (void*)commandBuffer;
        drawinfo.drawEvents.trigger();


       

    vkCmdEndRenderPass(commandBuffer);
}

void TourBillon::VulkanRHI::BeforeFrameDraw(float dt)
{
}
void TourBillon::VulkanRHI::AfterFrameDraw(float dt)
{
    m_current_frame_index = (m_current_frame_index + 1) % s_max_frames_in_flight;
}

void TourBillon::VulkanRHI::createTextureImage(void* imgdata, size_t imageSize, int imageWidth, int imageHeight, int texChannels, RHIImage*& image_buffer, RHIDeviceMemory*& buffer_memory)
{
    VulkanImage* vk_textureImage = new VulkanImage;
    VulkanDeviceMemory* vk_textureImageMemory = new VulkanDeviceMemory;

    if (!imgdata)
    {
        LOG_ERROR("loading texture image is NULL!")
    }

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(m_device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, imgdata, static_cast<size_t>(imageSize));
    vkUnmapMemory(m_device, stagingBufferMemory);

    createImage(imageWidth, imageHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vk_textureImage->image, vk_textureImageMemory->devicememory);

    VulkanCommandBuffer* vk_commandBuffer = (VulkanCommandBuffer*)beginSingleTimeCommands(0);
    transitionImageLayout(vk_commandBuffer->commandbuffer, vk_textureImage->image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(vk_commandBuffer->commandbuffer, stagingBuffer, vk_textureImage->image, static_cast<uint32_t>(imageWidth), static_cast<uint32_t>(imageHeight));
    transitionImageLayout(vk_commandBuffer->commandbuffer, vk_textureImage->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    endSingleTimeCommands(vk_commandBuffer, 0);

    vkDestroyBuffer(m_device, stagingBuffer, nullptr);
    vkFreeMemory(m_device, stagingBufferMemory, nullptr);

    image_buffer = vk_textureImage;
    buffer_memory = vk_textureImageMemory;
}



void TourBillon::VulkanRHI::waitFrameTime(float wait_deltaTime)
{
    if(wait_deltaTime)
    glfwWaitEventsTimeout(wait_deltaTime);
}

TourBillon::RHICommandBuffer* TourBillon::VulkanRHI::getCommandBuffer(uint32_t windowindex)
{
    return &m_commandBuffers[windowindex][m_current_frame_index];
}

void TourBillon::VulkanRHI::DrawViewport(RHIDrawInfo& draw_info)
{
    VkCommandBuffer commandBuffer = m_commandBuffers[draw_info.windowIndex][m_current_frame_index].commandbuffer;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)m_wapChainExtent[draw_info.windowIndex].width;
    viewport.height = (float)m_wapChainExtent[draw_info.windowIndex].height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = m_wapChainExtent[draw_info.windowIndex];
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

}

void TourBillon::VulkanRHI::DrawMesh(RHIDrawInfo& draw_info, RHIDrawMeshInfo& draw_mesh_info)
{
	VkCommandBuffer commandBuffer = m_commandBuffers[draw_info.windowIndex][m_current_frame_index].commandbuffer;

    std::vector<VkBuffer> vertexBuffer;
	VulkanBuffer* vk_vertex_buffer = dynamic_cast<VulkanBuffer*>(draw_mesh_info.vertex_buffer->buffer);
	
    VkBuffer vertexBuffers[] = { vk_vertex_buffer->buffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    VulkanBuffer* vk_index_buffer = dynamic_cast<VulkanBuffer*>(draw_mesh_info.index_buffer->buffer);

	vkCmdBindIndexBuffer(commandBuffer, vk_index_buffer->buffer, 0, VK_INDEX_TYPE_UINT32);

    VulkanPipeline* vk_pipeline = dynamic_cast<VulkanPipeline*>(draw_info.pipeline);
    VulkanDescriptorSet* vk_descriptorset = dynamic_cast<VulkanDescriptorSet*>(draw_info.descriptor_sets[m_current_frame_index]);
    for(int i=0;i< draw_mesh_info.uboDynamicOffsets.size();i++)
    {
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipeline->pipelinelayout, 0, 1, &vk_descriptorset->descriptor, 1, &draw_mesh_info.uboDynamicOffsets[i]);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(draw_mesh_info.indices_count), 1, 0, 0, 0);
    }
    //vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_pipeline->pipelinelayout, 0, 1, &vk_descriptorset->descriptor, draw_info.uboDynamicOffsets.size(), draw_info.uboDynamicOffsets.data());

	
}

void TourBillon::VulkanRHI::DrawDebug()
{
    //VkCommandBuffer commandBuffer = m_commandBuffers[m_current_frame_index].commandbuffer;
    //
    //VkViewport viewport{};
    //viewport.x = 0.0f;
    //viewport.y = 0.0f;
    //viewport.width = (float)m_wapChainExtent.width;
    //viewport.height = (float)m_wapChainExtent.height;
    //viewport.minDepth = 0.0f;
    //viewport.maxDepth = 1.0f;
    //vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    //
    //VkRect2D scissor{};
    //scissor.offset = { 0, 0 };
    //scissor.extent = m_wapChainExtent;
    //vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    //
    //vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}

void TourBillon::VulkanRHI::createBuffer(RHIDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(m_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		LOG_ERROR("failed to create buffer!");
	}

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        LOG_ERROR("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(m_device, buffer, bufferMemory, 0);
}

void TourBillon::VulkanRHI::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    for (int iwindow = 0; iwindow < m_windowSize; iwindow++)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_commandPools[iwindow];
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_graphicsQueue);

        vkFreeCommandBuffers(m_device, m_commandPools[iwindow], 1, &commandBuffer);
    }

}

void TourBillon::VulkanRHI::createInstance()
{
    // validation layer will be enabled in debug mode
    if (m_enable_validation_Layers && !checkValidationLayerSupport())
    {
        LOG_WARNING("validation layers requested, but not available!");
        m_enable_validation_Layers = false;
    }

    m_vulkan_api_version = VK_API_VERSION_1_3;

    

    // app info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "piccolo_renderer";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Piccolo";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = m_vulkan_api_version;

    // create info
    VkInstanceCreateInfo instance_create_info{};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &appInfo; // the appInfo is stored here

    auto extensions = getRequiredExtensions();
    instance_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instance_create_info.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (m_enable_validation_Layers)
    {
        instance_create_info.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
        instance_create_info.ppEnabledLayerNames = m_validation_layers.data();
    
        populateDebugMessengerCreateInfo(debugCreateInfo);
        instance_create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        instance_create_info.enabledLayerCount = 0;
        instance_create_info.pNext = nullptr;
    }

    // create m_vulkan_context._instance
    if (vkCreateInstance(&instance_create_info, nullptr, &m_instance) != VK_SUCCESS)
    {
        LOG_WARNING("failed to create instance!");
    }
}

void TourBillon::VulkanRHI::initializeDebugMessenger()
{
}

void TourBillon::VulkanRHI::createWindowSurface()
{
    m_surfaces.resize(m_vkWindows.size(), nullptr);
    int index = 0;
    for(auto window:m_vkWindows)
    {
        if (glfwCreateWindowSurface(m_instance, window->getWindow(), nullptr, &m_surfaces[index]) != VK_SUCCESS)
        {
            LOG_WARNING("failed to create window surface!");
        }
        index++;
    }
}

void TourBillon::VulkanRHI::initializePhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        LOG_WARNING("Failed to find a GPU with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            m_physicalDevice = device;
            break;
        }
    }

    if (m_physicalDevice == VK_NULL_HANDLE) {
        LOG_WARNING("Failed to find a suitable GPU!");
    }
}

void TourBillon::VulkanRHI::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.robustBufferAccess = VK_TRUE; // 启用 robustBufferAccess.robustBufferAccess = VK_TRUE; // 启用图像的安全访问
    //检查
    VkPhysicalDeviceRobustness2FeaturesEXT robustnessFeatures = {};
    robustnessFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT;
    robustnessFeatures.robustBufferAccess2 = VK_TRUE; // 启用缓冲区的安全访问
    robustnessFeatures.robustImageAccess2 = VK_TRUE; // 启用图像的安全访问

    VkPhysicalDeviceFeatures2 features2 = {};
    features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features2.pNext = &robustnessFeatures;
    features2.features.robustBufferAccess = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pNext = &features2;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    //createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.pEnabledFeatures = NULL;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = m_deviceExtensions.data();

    if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        LOG_WARNING("Failed to create logical device!");
    }

    vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);
}

void TourBillon::VulkanRHI::createCommandPool()
{
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    for (int iwindow = 0; iwindow < m_windowSize; iwindow++)
    {
        if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPools[iwindow]) != VK_SUCCESS) {
            LOG_WARNING("Failed to create command pool!");
        }
    }
}

void TourBillon::VulkanRHI::createCommandBuffers()
{
    VkCommandBufferAllocateInfo allocInfo{};

    VkCommandBuffer tmp_commandbuffer[s_max_frames_in_flight];

    for (int iwindow = 0; iwindow < m_windowSize; iwindow++)
    {
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPools[iwindow];
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = s_max_frames_in_flight;

        if (vkAllocateCommandBuffers(m_device, &allocInfo, tmp_commandbuffer) != VK_SUCCESS) {
            LOG_WARNING("failed to allocate command buffers!");
        }
    
        for (int i = 0; i < s_max_frames_in_flight; i++)
        {
            m_commandBuffers[iwindow][i].commandbuffer = tmp_commandbuffer[i];
        }
    }
}

void TourBillon::VulkanRHI::createDescriptorPool()
{
    TBAlignedArray<VkDescriptorPoolSize> poolSizes(2);
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(s_max_frames_in_flight);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(s_max_frames_in_flight);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(s_max_frames_in_flight);
    //poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

    if (vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
        LOG_WARNING("failed to create descriptor pool!");
    }
}

void TourBillon::VulkanRHI::createSyncPrimitives()
{
}

void TourBillon::VulkanRHI::createAssetAllocator()
{
}

void TourBillon::VulkanRHI::createSwapchain(uint32_t index)
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice, index);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities,index);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surfaces[index];

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
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

    if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain[index]) != VK_SUCCESS) {
        LOG_ERROR("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(m_device, m_swapChain[index], &imageCount, nullptr);
    m_swapChainImages[index].apply(imageCount);
    vkGetSwapchainImagesKHR(m_device, m_swapChain[index], &imageCount, m_swapChainImages[index].data());

    m_swapChainImageFormat = surfaceFormat.format;
    m_wapChainExtent[index] = extent;
}

bool TourBillon::VulkanRHI::createRenderPass(const RHIRenderPassCreateInfo* pCreateInfo, RHIRenderPass*& pRenderPass)
{
    // attachment convert
    TBAlignedArray<VkAttachmentDescription> attachments(pCreateInfo->attachments.size());
    TBAlignedArray<VkAttachmentReference> attachmentRefs(pCreateInfo->attachments.size());
    for (int i = 0; i < pCreateInfo->attachments.size(); i++)
    {
        RHIRenderPassCreateInfo::AttachmentEntry color_entry = pCreateInfo->attachments[i];

        VkAttachmentDescription itAttachment{};
        itAttachment.flags = i;
        itAttachment.format = m_swapChainImageFormat;// color_entry.RenderTarget->getFormat();
        itAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        itAttachment.loadOp = VulkanRHIResource::RHIToVkAttachmentLoadOp(color_entry.LoadAction);
        itAttachment.storeOp = VulkanRHIResource::RHIToVkAttachmentStoreOp(color_entry.StoreAction);
        itAttachment.stencilLoadOp = VulkanRHIResource::RHIToVkAttachmentLoadOp(color_entry.stencilLoadAction);
        itAttachment.stencilStoreOp = VulkanRHIResource::RHIToVkAttachmentStoreOp(color_entry.stencilStoreAction);
        itAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        itAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        attachments[i] = itAttachment;

        VkAttachmentReference itAttachmentRef{};
        itAttachmentRef.attachment = i;
        itAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;//

        attachmentRefs[i] = itAttachmentRef;
    }
    TBAlignedArray<VkSubpassDescription> subpasses(pCreateInfo->subpasses.size());
    TBAlignedArray<void*> sub_attachments_ref_copy(pCreateInfo->subpasses.size());
    for (int i = 0; i < pCreateInfo->subpasses.size(); i++)
    {
        RHIRenderPassCreateInfo::SubpassEntry subpass_entry = pCreateInfo->subpasses[i];
        
        for (int index = 0; index < subpass_entry.colorAttachment.size(); index++)
        {
            sub_attachments_ref_copy[index] = (void*)&attachmentRefs[subpass_entry.colorAttachment[index]];
        }

        VkSubpassDescription itsubpass{};
        itsubpass.pipelineBindPoint = VulkanRHIResource::RHIToVkPipelineBindPoint(subpass_entry.bindPipelineState);// VK_PIPELINE_BIND_POINT_GRAPHICS;
        itsubpass.colorAttachmentCount = sub_attachments_ref_copy.size();
        itsubpass.pColorAttachments = (VkAttachmentReference*)*sub_attachments_ref_copy.data();//内存泄露

        subpasses[i] = itsubpass;

    }

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = attachments.size();
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = subpasses.size();
    renderPassInfo.pSubpasses = subpasses.data();

    VulkanRenderPass* vk_render_pass = new VulkanRenderPass;//dynamic_cast<VulkanRenderPass*>(pRenderPass);

    if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &vk_render_pass->renderpass) != VK_SUCCESS) {
        LOG_WARNING("failed to create render pass!");
        return false;
    }
    pRenderPass = vk_render_pass;

    return true;
}

bool TourBillon::VulkanRHI::createDescriptorSetLayout(const RHIDescriptorSetLayoutCreateInfo* pCreateInfo, RHIDescriptorSetLayout*& pSetLayout)
{
    VulkanDescriptorSetLayout* vk_layout =  new VulkanDescriptorSetLayout;
    
    
    std::vector<VkDescriptorSetLayoutBinding> uboLayoutBinding(pCreateInfo->bindingCount);

    for (int i = 0; i < pCreateInfo->bindingCount; ++i)
    {
        const auto& rhi_layout_binding_element = pCreateInfo->pBindings[i];
        auto& ubo_layout_binding_element = uboLayoutBinding[i];
        ubo_layout_binding_element.binding = rhi_layout_binding_element.binding;
        ubo_layout_binding_element.descriptorCount = rhi_layout_binding_element.descriptorCount;
        ubo_layout_binding_element.descriptorType = (VkDescriptorType)rhi_layout_binding_element.descriptorType;
        ubo_layout_binding_element.pImmutableSamplers = nullptr;
        ubo_layout_binding_element.stageFlags = VulkanRHIResource::RHIToVkShaderStageFlagBits(rhi_layout_binding_element.stageFlags);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = pCreateInfo->bindingCount;
    layoutInfo.pBindings = uboLayoutBinding.data();
    //layoutInfo.flags = VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT;

    if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &vk_layout->layout) != VK_SUCCESS) {
        LOG_ERROR("failed to create descriptor set layout!");
        return false;
    }

    pSetLayout = vk_layout;

    return true;
}

bool TourBillon::VulkanRHI::AllocateDescriptorSets(const RHIDescriptorSetAllocateInfo* pAllocateInfo, RHIDescriptorSet*& pDescriptorSets)
{
    VulkanDescriptorSet* vk_descriptor = new VulkanDescriptorSet;
    VulkanDescriptorSetLayout* vk_layout = dynamic_cast<VulkanDescriptorSetLayout*>(pAllocateInfo->layout);

    std::vector<VkDescriptorSetLayout> layouts(s_max_frames_in_flight, vk_layout->layout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = 1;// static_cast<uint32_t>(s_max_frames_in_flight);
    allocInfo.pSetLayouts = &vk_layout->layout;

    
    VkResult result = vkAllocateDescriptorSets(m_device, &allocInfo, &vk_descriptor->descriptor);
    if (result != VK_SUCCESS) {
        LOG_ERROR("failed to allocate descriptor sets!");
    }

    pDescriptorSets = vk_descriptor;
    return true;
}

bool TourBillon::VulkanRHI::updateDescriptorSets(RHIUpdatesDescriptorSetsInfo& writeinfo)
{
    TBVector<VkWriteDescriptorSet> descriptorWrite(writeinfo.write_info.size());
    TBVector<VkDescriptorBufferInfo> descriptorBufferInfo(writeinfo.write_info.size());
    uint32_t index = 0;
    for (auto itr : writeinfo.write_info)
    {
        VkDescriptorBufferInfo bufferInfo{};

        VulkanBuffer* vk_buffer = dynamic_cast<VulkanBuffer*>(itr.buffer);
        VulkanDescriptorSet* vk_descriptor = dynamic_cast<VulkanDescriptorSet*>(itr.descriptorset);
        descriptorBufferInfo[index].buffer = vk_buffer->buffer;
        descriptorBufferInfo[index].offset = itr.offset;
        descriptorBufferInfo[index].range = itr.range;
		//bufferInfo.buffer = vk_buffer->buffer;
		//bufferInfo.offset = itr.offset;
		//bufferInfo.range = itr.range;

        descriptorWrite[index].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite[index].dstSet = vk_descriptor->descriptor;
        descriptorWrite[index].dstBinding = itr.bindingindex;
        descriptorWrite[index].dstArrayElement = 0;
        descriptorWrite[index].descriptorType = static_cast<VkDescriptorType>(itr.descriptorType);
        descriptorWrite[index].descriptorCount = 1;
        descriptorWrite[index].pBufferInfo = &descriptorBufferInfo[index];

        index++;
    }

    vkUpdateDescriptorSets(m_device, descriptorWrite.size(), descriptorWrite.data(), 0, nullptr);

    return true;
}

void TourBillon::VulkanRHI::createSwapchainImageViews(uint32_t index)
{
    m_swapChainImageViews[index].apply(m_swapChainImages[index].size());

    for (size_t i = 0; i < m_swapChainImages[index].size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_swapChainImages[index][i];
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

        VulkanImageView* swapChainImageViews = new VulkanImageView;
        

        if (vkCreateImageView(m_device, &createInfo, nullptr, &swapChainImageViews->imageview) != VK_SUCCESS) {
            LOG_WARNING("failed to create image views!");
        }
        m_swapChainImageViews[index][i] = swapChainImageViews;
    }
    return;
}

bool TourBillon::VulkanRHI::createGraphicsPipeline(const RHIPipelineCreateInfo* pCreateInfo, RHIPipeline*& pPipeline)
{
    VulkanPipeline* vk_pipeline = new VulkanPipeline;// dynamic_cast<VulkanPipeline*>(pPipeline);

    TBAlignedArray<VkPipelineShaderStageCreateInfo> shaderStages;
    shaderStages.apply(pCreateInfo->shaders.size());
    std::vector<VkShaderModule> loading_shaderModules;
    for (int i = 0; i < pCreateInfo->shaders.size(); i++)
    {
        RHIPipelineCreateInfo::ShaderEntry shader_entry = pCreateInfo->shaders[i];
        auto shaderCode = readFile(shader_entry.shaderpath);
        VkShaderModule shaderModule = createShaderModule(shaderCode);
        loading_shaderModules.push_back(shaderModule);

        VkPipelineShaderStageCreateInfo shaderStageInfo{};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = VulkanRHIResource::RHIToVkShaderStageFlagBits(shader_entry.shadertype);
        shaderStageInfo.module = loading_shaderModules[i];
        shaderStageInfo.pName = "main";

        shaderStages[i] = shaderStageInfo;
    }

    //绑定描述符
    VkVertexInputBindingDescription bindingDescription = getVertexBindingDescription();
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions = getVertexAttributeDescriptions();

    //定义顶点输入的格式和绑定方式
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();


    //顶点组装的方式
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    //视口和剪裁区域
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    //深度测试
    VkPipelineDepthStencilStateCreateInfo depth_info = {};
    depth_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_info.depthCompareOp = VK_COMPARE_OP_ALWAYS;

    //光栅化状态
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    //多重采样的配置
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    //颜色混合的配置
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    //在管线外动态设置的状态
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = pCreateInfo->descriptor_layouts.size();
    //pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    TBVector<VkDescriptorSetLayout> vk_layouts(pCreateInfo->descriptor_layouts.size());
    for (int i = 0; i < pCreateInfo->descriptor_layouts.size(); i++)
    {
        vk_layouts[i] = dynamic_cast<VulkanDescriptorSetLayout*>(pCreateInfo->descriptor_layouts[i])->layout;
    }
    pipelineLayoutInfo.pSetLayouts = vk_layouts.data();

    if (vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &vk_pipeline->pipelinelayout) != VK_SUCCESS) {
        LOG_WARNING("failed to create pipeline layout!");
    }

    VulkanRenderPass* vk_render_pass = dynamic_cast<VulkanRenderPass*>(pCreateInfo->renderpass);

    //VkPipelineRobustnessBufferBehaviorCreateInfoEXT robustnessBufferBehavior = {};
    //robustnessBufferBehavior.sType = VK_STRUCTURE_TYPE_PIPELINE_ROBUSTNESS_BUFFER_BEHAVIOR_CREATE_INFO_EXT;
    //robustnessBufferBehavior.bufferAccessBehavior = VK_PIPELINE_ROBUSTNESS_BUFFER_ACCESS_ROBUST_BUFFER_ACCESS_EXT;


    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = pCreateInfo->shaders.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDepthStencilState = &depth_info;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = vk_pipeline->pipelinelayout;
    pipelineInfo.renderPass = vk_render_pass->renderpass;
    pipelineInfo.subpass = pCreateInfo->subpassIndex;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    
    if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vk_pipeline->pipeline) != VK_SUCCESS) {
        LOG_ERROR("failed to create graphics pipeline!");
        return false;
    }
    pPipeline = vk_pipeline;
    for (int i = 0; i < loading_shaderModules.size(); i++)
    {
        vkDestroyShaderModule(m_device, loading_shaderModules[i], nullptr);
    }
    VulkanPipeline* vpipeline = dynamic_cast<VulkanPipeline*>(pPipeline);
    return true;
}

bool TourBillon::VulkanRHI::createFrameBuffer(const RHIFramebufferCreateInfo* pCreateInfo, RHIFramebuffer*& pframeBuffer)
{
    VulkanRenderPass* vk_render_pass = dynamic_cast<VulkanRenderPass*>(pCreateInfo->renderpass);
    VulkanFramebuffer* vk_frame_buffer = new VulkanFramebuffer;

    TBAlignedArray<VkImageView> attachments(pCreateInfo->imageviews.size());
    for (int i = 0; i < pCreateInfo->imageviews.size(); i++)
    {
        VulkanImageView* vk_image_view = dynamic_cast<VulkanImageView*>(pCreateInfo->imageviews[i]);
        attachments[i] = vk_image_view->imageview;// m_swapChainImageViews[i];
    }

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = vk_render_pass->renderpass;
    framebufferInfo.attachmentCount = attachments.size();
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = m_wapChainExtent[pCreateInfo->windowIndex].width;
    framebufferInfo.height = m_wapChainExtent[pCreateInfo->windowIndex].height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &vk_frame_buffer->framebuffer) != VK_SUCCESS) {
        LOG_WARNING("failed to create framebuffer!");
    }

    pframeBuffer = vk_frame_buffer;

    return true;
}
void TourBillon::VulkanRHI::createFrameBufferImageAndView(uint32_t index)
{
    VulkanCommandBuffer* vk_commandBuffer = (VulkanCommandBuffer*)beginSingleTimeCommands(index);
    for (int frame = 0; frame < s_max_frames_in_flight; frame++)
        transitionImageLayout(vk_commandBuffer->commandbuffer, m_swapChainImages[index][frame], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    endSingleTimeCommands(vk_commandBuffer, index);
}
void TourBillon::VulkanRHI::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
    {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(m_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(m_device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            LOG_ERROR("failed to allocate image memory!");
        }

        vkBindImageMemory(m_device, image, imageMemory, 0);
    }
}
void TourBillon::VulkanRHI::copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}
VkImageView TourBillon::VulkanRHI::createImageView(VkImage image, VkFormat format)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(m_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        LOG_ERROR("failed to create texture image view!");
    }

    return imageView;
}
bool TourBillon::VulkanRHI::createSemaphore()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    for(int iwindow = 0;iwindow<m_windowSize; iwindow++)
    {
        for (int i = 0; i < s_max_frames_in_flight; i++)
        {
            if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore[iwindow][i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore[iwindow][i]) != VK_SUCCESS ||
                vkCreateFence(m_device, &fenceInfo, nullptr, &m_inFlightFence[iwindow][i]) != VK_SUCCESS) {
                LOG_WARNING("failed to create synchronization objects for a frame!");
                return false;
            };
        }
    }
    return true;
}

VkShaderModule TourBillon::VulkanRHI::createShaderModule(const std::vector<char>& code)
{
    VkShaderModuleCreateInfo shader_module_create_info{};
    shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.codeSize = code.size();
    shader_module_create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shader_module;
    if (code.empty() || vkCreateShaderModule(m_device, &shader_module_create_info, nullptr, &shader_module) != VK_SUCCESS)
    {
        LOG_WARNING("failed to create shader module!");
        return VK_NULL_HANDLE;
    }
    return shader_module;
}

void TourBillon::VulkanRHI::updateBuffer(void* data, uint32_t windowindex, RHIBuffer* buffer, RHIDeviceSize totaloffset, RHIDeviceSize totalsize)
{
    VkCommandBuffer commandBuffer = m_commandBuffers[windowindex][m_current_frame_index].commandbuffer;
    VulkanBuffer* vk_buffer = dynamic_cast<VulkanBuffer*>(buffer);
    size_t chunkSize = 65536;
    for (size_t ioffset = 0; ioffset < totalsize - totaloffset; ioffset += chunkSize) {
        size_t isize = min(chunkSize, totalsize - totaloffset - ioffset);
        vkCmdUpdateBuffer(commandBuffer, vk_buffer->buffer, ioffset, isize, data);
    }
    
    //vkCmdUpdateBuffer(commandBuffer, vk_buffer->buffer, offset, size, data);
}

void TourBillon::VulkanRHI::createVertexBuffer(void* srcdata, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory)
{
    if (size == 0)return;
    VkDeviceSize bufferSize = size;
	VulkanBuffer* vk_buffer = new VulkanBuffer;// dynamic_cast<VulkanBuffer*>(buffer);
	VulkanDeviceMemory* vk_buffer_memory = new VulkanDeviceMemory;// dynamic_cast<VulkanDeviceMemory*>(buffer_memory);

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* dstdata;
    vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &dstdata);
    memcpy(dstdata, srcdata, (size_t)bufferSize);
    vkUnmapMemory(m_device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vk_buffer->buffer, vk_buffer_memory->devicememory);

    copyBuffer(stagingBuffer, vk_buffer->buffer, bufferSize);

    vkDestroyBuffer(m_device, stagingBuffer, nullptr);
    vkFreeMemory(m_device, stagingBufferMemory, nullptr);

	buffer = vk_buffer;
	buffer_memory = vk_buffer_memory;
}

void TourBillon::VulkanRHI::createIndexBuffer(void* srcdata, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory)
{
    if (size == 0)return;
    VkDeviceSize bufferSize = size;
    VulkanBuffer* vk_buffer = new VulkanBuffer;// dynamic_cast<VulkanBuffer*>(buffer);
    VulkanDeviceMemory* vk_buffer_memory = new VulkanDeviceMemory;// dynamic_cast<VulkanDeviceMemory*>(buffer_memory);

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* dstdata;
    vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &dstdata);
    memcpy(dstdata, srcdata, (size_t)bufferSize);
    vkUnmapMemory(m_device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vk_buffer->buffer, vk_buffer_memory->devicememory);

    copyBuffer(stagingBuffer, vk_buffer->buffer, bufferSize);

    vkDestroyBuffer(m_device, stagingBuffer, nullptr);
    vkFreeMemory(m_device, stagingBufferMemory, nullptr);

    buffer = vk_buffer;
    buffer_memory = vk_buffer_memory;
}

void TourBillon::VulkanRHI::createUniformBuffer(void* mapdata, RHIDeviceSize size, RHIBuffer*& buffer, RHIDeviceMemory*& buffer_memory)
{
    if (size == 0)return;
    VkDeviceSize bufferSize = size;
    VulkanBuffer* vk_buffer = new VulkanBuffer;// dynamic_cast<VulkanBuffer*>(buffer);
    VulkanDeviceMemory* vk_buffer_memory = new VulkanDeviceMemory;// dynamic_cast<VulkanDeviceMemory*>(buffer_memory);

    if (buffer)
        vkDestroyBuffer(m_device, dynamic_cast<VulkanBuffer*>(buffer)->buffer , nullptr);
    if(buffer_memory)
        vkFreeMemory(m_device, dynamic_cast<VulkanDeviceMemory*>(buffer_memory)->devicememory, nullptr);

    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vk_buffer->buffer, vk_buffer_memory->devicememory);

    void* dstdata;
    vkMapMemory(m_device, vk_buffer_memory->devicememory, 0, bufferSize, 0, &dstdata);
    memcpy(dstdata, mapdata, (size_t)bufferSize);
    vkUnmapMemory(m_device, vk_buffer_memory->devicememory);

    buffer = vk_buffer;
    buffer_memory = vk_buffer_memory;
}


bool TourBillon::VulkanRHI::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : m_validation_layers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}
void TourBillon::VulkanRHI::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}
bool TourBillon::VulkanRHI::isDeviceSuitable(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        for (int i = 0; i < m_surfaces.size(); i++)
        {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, i);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;

    //VkPhysicalDeviceProperties deviceProperties;
    //vkGetPhysicalDeviceProperties(device, &deviceProperties);
    //return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
}

TourBillon::VulkanRHI::SwapChainSupportDetails TourBillon::VulkanRHI::querySwapChainSupport(VkPhysicalDevice device, uint32_t index)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surfaces[index], &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surfaces[index], &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surfaces[index], &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surfaces[index], &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surfaces[index], &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR TourBillon::VulkanRHI::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}
VkExtent2D TourBillon::VulkanRHI::chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities, uint32_t index)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }
    else {
        int width, height;
        glfwGetFramebufferSize(m_vkWindows[index]->getWindow(), &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

VkPresentModeKHR TourBillon::VulkanRHI::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

TourBillon::VulkanRHI::QueueFamilyIndices TourBillon::VulkanRHI::findQueueFamilies(VkPhysicalDevice device)
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
        for (uint32_t iwindow = 0; iwindow < m_windowSize; ++iwindow) {
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surfaces[iwindow], &presentSupport);
            if (presentSupport == false)
                break;
        }

        //vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surfaces[index], &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

bool TourBillon::VulkanRHI::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

std::vector<const char*> TourBillon::VulkanRHI::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (m_enable_validation_Layers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

uint32_t TourBillon::VulkanRHI::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    LOG_WARNING("failed to find suitable memory type!");
    return 0;
}

void TourBillon::VulkanRHI::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

TourBillon::RHICommandBuffer* TourBillon::VulkanRHI::beginSingleTimeCommands(uint32_t windowindex)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPools[windowindex];
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(m_device, &allocInfo, &command_buffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &beginInfo);

    RHICommandBuffer* rhi_command_buffer = new VulkanCommandBuffer();
    ((VulkanCommandBuffer*)rhi_command_buffer)->commandbuffer = (command_buffer);
    return rhi_command_buffer;
}

void TourBillon::VulkanRHI::endSingleTimeCommands(RHICommandBuffer* command_buffer, uint32_t windowindex)
{
    VkCommandBuffer vk_command_buffer = ((VulkanCommandBuffer*)command_buffer)->commandbuffer;
    vkEndCommandBuffer(vk_command_buffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vk_command_buffer;

    vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_graphicsQueue);

    vkFreeCommandBuffers(m_device, m_commandPools[windowindex], 1, &vk_command_buffer);
    delete(command_buffer);
}

void TourBillon::VulkanRHI::cleanup()
{
    for (int iwindow = 0; iwindow < m_windowSize; iwindow++)
    {
        for (size_t i = 0; i < s_max_frames_in_flight; i++) {
            vkDestroySemaphore(m_device, m_renderFinishedSemaphore[iwindow][i], nullptr);
            vkDestroySemaphore(m_device, m_imageAvailableSemaphore[iwindow][i], nullptr);
            vkDestroyFence(m_device, m_inFlightFence[iwindow][i], nullptr);
        }
        vkDestroyCommandPool(m_device, m_commandPools[iwindow], nullptr);
        clearSwapchain(iwindow);
    }


    vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);
    
    vkDestroyPipeline(m_device, m_graphicsPipeline, nullptr);
	vkDestroyRenderPass(m_device, m_renderPass, nullptr);
    
    vkDestroyDevice(m_device, nullptr);
    vkDestroyInstance(m_instance, nullptr);
    if (m_enable_validation_Layers) {
        DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
    }
    for(auto window: m_vkWindows)
    glfwDestroyWindow(window->getWindow());
    glfwTerminate();
}

void TourBillon::VulkanRHI::clearSwapchain(uint32_t index)
{
    for (auto imageview : m_swapChainImageViews[index])
    {
        VulkanImageView* vk_imageview = dynamic_cast<VulkanImageView*>(imageview);
        if (vk_imageview)
        {
            vkDestroyImageView(m_device, vk_imageview->imageview, NULL);
        }
    }
    if (m_swapChain[index])
        vkDestroySwapchainKHR(m_device, m_swapChain[index], NULL); // also swapchain images
}

void TourBillon::VulkanRHI::destroyFramebuffer(RHIFramebuffer* framebuffer)
{
    VulkanFramebuffer* vk_frame_buffer = dynamic_cast<VulkanFramebuffer*>(framebuffer);
    if(vk_frame_buffer)
        vkDestroyFramebuffer(m_device, vk_frame_buffer->framebuffer, NULL);
}




