#include "TextureComponent.h"
#include "ECSManager.h"


TourBillon::TextureData* TourBillon::TexturePtr::loadData()
{
	//���ͼ����¼���IOϵͳ����
	ECSManager::Instance()->SendEvent(Events::LOAD_IMAGE, (void*)this);
	return dynamic_cast<TextureData*>(assetdata);
}

void TourBillon::TexturePtr::releaseData()
{
	ECSManager::Instance()->SendEvent(Events::RELEASE_IMAGE, (void*)this);
}
