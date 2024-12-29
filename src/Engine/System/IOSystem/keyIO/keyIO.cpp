#include "keyIO.h"
#include "ECSManager.h"
#include "Math/trans.hpp"

#include "components/ComponentData.h"
#include "components/KeysComponent.h"
#include "components/CameraComponent.h"
#include "components/WindowComponent.h"
#include "components/GeometryComponent.h"
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
		//auto& camera = ECSManager::Instance()->GetComponent<Camera3D>(window.camera);
		auto& trans = ECSManager::Instance()->GetComponent<Transfrom>(window.camera);
		trans.move(trans.getright().normalizedCopy() * -0.1);
	}
	if (buttons.keyvalue[GLFW_KEY_W])
	{
		auto& trans = ECSManager::Instance()->GetComponent<Transfrom>(window.camera);
		trans.move(trans.getforward().normalizedCopy() * 0.1);
	}
	if (buttons.keyvalue[GLFW_KEY_D])
	{
		auto& trans = ECSManager::Instance()->GetComponent<Transfrom>(window.camera);
		trans.move(trans.getright().normalizedCopy() * 0.1);
	}
	if (buttons.keyvalue[GLFW_KEY_S])
	{
		auto& trans = ECSManager::Instance()->GetComponent<Transfrom>(window.camera);
		trans.move(trans.getforward().normalizedCopy() * -0.1);
	}
	if (buttons.keyvalue[GLFW_KEY_Q])
	{
		static int index = 2;
		ECSManager::Instance()->RemoveComponent<GeometryShared>(index);
		ECSManager::Instance()->RemoveComponent<Transfrom>(index);
		index++;
	}
}

void TourBillon::keyIO::updateMouseState(const CEvent& event)
{
	Entity window_entity = (Entity)event.event_data;
	auto& mouse = ECSManager::Instance()->GetComponent<Mouse>(window_entity);
	auto& window = ECSManager::Instance()->GetComponent<RenderWindow>(mouse.baseWindow);
	if (mouse.pressRight)
	{
		auto& trans = ECSManager::Instance()->GetComponent<Transfrom>(window.camera);
		trans.rotate(TBMath::Vec3(-mouse.mouseOffset.y * 0.1, mouse.mouseOffset.x * 0.1, 0.f));
		//camera.rotation(0.001, 0.001, 0.f);
	}
}

