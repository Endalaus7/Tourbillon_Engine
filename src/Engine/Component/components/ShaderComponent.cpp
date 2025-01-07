#include "ShaderComponent.h"
#include "ECSManager.h"

TourBillon::ShaderData* TourBillon::ShaderPtr::loadData()
{
	ECSManager::Instance()->SendEvent(Events::LOAD_SHADER, (void*)this);
	return dynamic_cast<ShaderData*>(assetdata);
}

void TourBillon::ShaderPtr::releaseData()
{
}
