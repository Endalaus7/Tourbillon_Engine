#include "RenderComponent.h"
#include "WindowComponent.h"
#include "ECSManager.h"

void TourBillon::RenderPort::setParent(Entity parent_entity)
{
	ECSWindow& parentwindow = ECSManager::Instance()->GetComponent<ECSWindow>(parent_entity);

	parentwindow

}
