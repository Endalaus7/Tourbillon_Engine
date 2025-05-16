#pragma once

#include <memory>
#include "rhi_window.h"


TourBillon::RHIWindow::~RHIWindow()
{
    //ECSManager::Instance()->RemoveComponent<ECSWindow>(m_window_entity);
    //ECSManager::Instance()->RemoveComponent<Buttons>(m_window_entity);
    //ECSManager::Instance()->RemoveComponent<Mouse>(m_window_entity);
}

void TourBillon::RHIWindow::initialize(RHIWindowInfo& initialize_info)
{
    //m_index = initialize_info.index;
    m_width = initialize_info.width;
    m_height = initialize_info.height;

    m_window_entity = initialize_info.window_entity;

    //window的ecs属性不应该由rhiwindow赋予，移出

    //m_window_entity = ECSManager::Instance()->CreateEntity();
    //ECSWindow ecswindow;
    //ecswindow.width = m_width;
    //ecswindow.height = m_height;
    //ecswindow.index = m_index;
    //ecswindow.title = initialize_info.title;
    //ecswindow.isFullscreen = initialize_info.is_fullscreen;
    //ECSManager::Instance()->AddComponent<ECSWindow>(m_window_entity, ecswindow);

    //Buttons button;
    //ECSManager::Instance()->AddComponent<Buttons>(m_window_entity, button);
    
    //Mouse mouse;
    //mouse.baseWindow = m_window_entity;
    //ECSManager::Instance()->AddComponent<Mouse>(m_window_entity, mouse);

}

//void TourBillon::RHIWindow::setCamera(Entity camera_entity)
//{
//	auto& window = ECSManager::Instance()->GetComponent<ECSWindow>(m_window_entity);
//
//	window.camera = camera_entity;
//}