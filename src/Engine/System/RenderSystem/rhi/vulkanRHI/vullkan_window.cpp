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

void TourBillon::VulkanWindow::initialize(WindowCreateInfo& initialize_info)
{
    RHIWindow::initialize(initialize_info);
    if (!glfwInit())
    {
        //LOG(__FUNCTION__, "failed to initialize GLFW");
        return;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    auto sharewindow = std::dynamic_pointer_cast<VulkanWindow>(initialize_info.sharedwindow);
    m_window = glfwCreateWindow(initialize_info.width, initialize_info.height, initialize_info.title, nullptr, sharewindow ? sharewindow->getWindow() : nullptr);
    if (!m_window)
    {
        //LOG_FATAL(__FUNCTION__, "failed to create window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);

    // Setup input callbacks
    //glfwSetWindowUserPointer(m_window, this);//后续写一个vulkan_window类型，承载此处的回调
    glfwSetKeyCallback(m_window, keyCallback);
    //glfwSetCharCallback(m_window, charCallback);
    //glfwSetCharModsCallback(m_window, charModsCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    glfwSetCursorPosCallback(m_window, cursorPosCallback);
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


void TourBillon::VulkanWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto& buttons = ECSManager::Instance()->GetComponent<Buttons>(RHIWindow::m_window_entity);
    buttons.baseWindow = RHIWindow::m_window_entity;
    if(action == GLFW_PRESS)
        buttons.keyvalue.set(key);
    else if(action == GLFW_RELEASE)
        buttons.keyvalue.reset(key);
    ECSManager::Instance()->SendEvent(Events::KEY_CHANGE, (void*)RHIWindow::m_window_entity);
}



void TourBillon::VulkanWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    auto& mouse = ECSManager::Instance()->GetComponent<Mouse>(RHIWindow::m_window_entity);
    if (button == GLFW_MOUSE_BUTTON_LEFT)
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
            mouse.pressLeft = true;
        }
        else if (action == GLFW_RELEASE)
        {
            glfwSetCursorPos(window, m_lockx, m_locky);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_islock = false;
            mouse.pressLeft = false;
        }

    }

}

void TourBillon::VulkanWindow::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto& mouse = ECSManager::Instance()->GetComponent<Mouse>(RHIWindow::m_window_entity);
    mouse.baseWindow = RHIWindow::m_window_entity;
    mouse.mousePos = TBMath::Vec2(xpos, ypos);
    mouse.mouseOffset = TBMath::Vec2(xpos - m_lastx, ypos - m_lasty);
    
    LOG_DEBUG(std::to_string(mouse.mouseOffset.x) + "," + std::to_string(mouse.mouseOffset.y));

    ECSManager::Instance()->SendEvent(Events::MOUSE_MOVE, (void*)RHIWindow::m_window_entity);

    m_lastx = xpos;
    m_lasty = ypos;
}
