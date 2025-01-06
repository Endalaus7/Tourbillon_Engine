#include "MaterialComponent.h"
#include "ECSManager.h"


TourBillon::Texture* TourBillon::TextureShared::loadData()
{
	//发送加载事件，IO系统处理
	ECSManager::Instance()->SendEvent(Events::LOAD_IMAGE, (void*)this);
	return dynamic_cast<Texture*>(assetdata);
}

void TourBillon::TextureShared::releaseData()
{
	ECSManager::Instance()->SendEvent(Events::RELEASE_IMAGE, (void*)this);
}
