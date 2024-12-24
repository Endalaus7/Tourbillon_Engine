#pragma once

#include <memory>
#include "rhi_window.h"

TourBillon::Entity TourBillon::RHIWindow::m_window_entity = 0;

TourBillon::RHIWindow::~RHIWindow()
{
    ECSManager::Instance()->RemoveComponent<RenderWindow>(m_window_entity);
    ECSManager::Instance()->RemoveComponent<Buttons>(m_window_entity);
    ECSManager::Instance()->RemoveComponent<Mouse>(m_window_entity);
}

void TourBillon::RHIWindow::initialize(WindowCreateInfo& initialize_info)
{
    m_index = initialize_info.index;
    m_width = initialize_info.width;
    m_height = initialize_info.height;


    m_window_entity = ECSManager::Instance()->CreateEntity();
    RenderWindow ecswindow;
    ecswindow.width = m_width;
    ecswindow.height = m_height;
    ecswindow.index = m_index;
    ecswindow.title = initialize_info.title;
    ecswindow.isFullscreen = initialize_info.is_fullscreen;
    ECSManager::Instance()->AddComponent<RenderWindow>(m_window_entity, ecswindow);

    Buttons button;
    ECSManager::Instance()->AddComponent<Buttons>(m_window_entity, button);

    Mouse mouse;
    mouse.baseWindow = m_window_entity;
    ECSManager::Instance()->AddComponent<Mouse>(m_window_entity, mouse);

}

void TourBillon::RHIWindow::setCamera(Entity camera_entity)
{
	auto& window = ECSManager::Instance()->GetComponent<RenderWindow>(m_window_entity);

	window.camera = camera_entity;
}