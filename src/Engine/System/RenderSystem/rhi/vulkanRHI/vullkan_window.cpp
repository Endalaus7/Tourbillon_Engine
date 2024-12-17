#include "vullkan_window.h"

void TourBillon::VulkanWindow::initialize(WindowCreateInfo& initialize_info)
{
    if (!glfwInit())
    {
        //LOG(__FUNCTION__, "failed to initialize GLFW");
        return;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


    m_window = glfwCreateWindow(initialize_info.width, initialize_info.height, initialize_info.title, nullptr, nullptr);
    if (!m_window)
    {
        //LOG_FATAL(__FUNCTION__, "failed to create window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);

    m_width = initialize_info.width;
    m_height = initialize_info.height;

    // Setup input callbacks
    //glfwSetWindowUserPointer(m_window, this);//后续写一个vulkan_window类型，承载此处的回调
    //glfwSetKeyCallback(m_window, keyCallback);
    //glfwSetCharCallback(m_window, charCallback);
    //glfwSetCharModsCallback(m_window, charModsCallback);
    //glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    //glfwSetCursorPosCallback(m_window, cursorPosCallback);
    //glfwSetCursorEnterCallback(m_window, cursorEnterCallback);
    //glfwSetScrollCallback(m_window, scrollCallback);
    //glfwSetDropCallback(m_window, dropCallback);
    //glfwSetWindowSizeCallback(m_window, windowSizeCallback);
    //glfwSetWindowCloseCallback(m_window, windowCloseCallback);

   // glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
}

const TourBillon::WindowCreateInfo& TourBillon::VulkanWindow::getWindowInfo()
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
