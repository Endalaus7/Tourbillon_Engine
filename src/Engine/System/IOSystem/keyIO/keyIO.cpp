#include "keyIO.h"
#include "ECSManager.h"
#include "Math/trans.hpp"

#include "components/ComponentData.h"
#include "components/KeysComponent.h"
#include "components/CameraComponent.h"
#include "components/WindowComponent.h"
#include "GLFW/glfw3.h"

void TourBillon::keyIO::init()
{
	ECSManager::Instance()->AddListener(Events::KEY_CHANGE, METHOD_LISTENER(keyIO::updateKeyState));
	ECSManager::Instance()->AddListener(Events::MOUSE_MOVE, METHOD_LISTENER(keyIO::updateMouseState));
}

void TourBillon::keyIO::updateKeyState(const CEvent& event)
{
	Entity window_entity = (Entity)event.event_data;
	auto& buttons = ECSManager::Instance()->GetComponent<Buttons>(window_entity);
	
	auto& window = ECSManager::Instance()->GetComponent<RenderWindow>(buttons.baseWindow);
	
	if (buttons.keyvalue[GLFW_KEY_A])
	{
		auto& camera = ECSManager::Instance()->GetComponent<Camera3D>(window.camera);
		camera.move(camera.getDirection().normalizedCopy().cross(camera.up.normalizedCopy()) * -0.1);
	}
	if (buttons.keyvalue[GLFW_KEY_W])
	{
		auto& camera = ECSManager::Instance()->GetComponent<Camera3D>(window.camera);
		camera.move(camera.getDirection().normalizedCopy() * 0.1);
	}
	if (buttons.keyvalue[GLFW_KEY_D])
	{
		auto& camera = ECSManager::Instance()->GetComponent<Camera3D>(window.camera);
		camera.move(camera.getDirection().normalizedCopy().cross(camera.up.normalizedCopy()) * 0.1);
	}
	if (buttons.keyvalue[GLFW_KEY_S])
	{
		auto& camera = ECSManager::Instance()->GetComponent<Camera3D>(window.camera);
		camera.move(camera.getDirection().normalizedCopy() * -0.1);
	}
}

void TourBillon::keyIO::updateMouseState(const CEvent& event)
{
	Entity window_entity = (Entity)event.event_data;
	auto& mouse = ECSManager::Instance()->GetComponent<Mouse>(window_entity);
	auto& window = ECSManager::Instance()->GetComponent<RenderWindow>(mouse.baseWindow);
	if (mouse.pressRight)
	{
		auto& camera = ECSManager::Instance()->GetComponent<Camera3D>(window.camera);
		camera.rotation(TBMath::ToRadians(-mouse.mouseOffset.y) * 0.1, TBMath::ToRadians(mouse.mouseOffset.x) * 0.1, 0.f);
		//camera.rotation(0.001, 0.001, 0.f);
	}
}

