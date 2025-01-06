#include "MaterialComponent.h"
#include "ECSManager.h"


TourBillon::Texture* TourBillon::TextureShared::loadData()
{
	//���ͼ����¼���IOϵͳ����
	ECSManager::Instance()->SendEvent(Events::LOAD_IMAGE, (void*)this);
	return dynamic_cast<Texture*>(assetdata);
}

void TourBillon::TextureShared::releaseData()
{
	ECSManager::Instance()->SendEvent(Events::RELEASE_IMAGE, (void*)this);
}
