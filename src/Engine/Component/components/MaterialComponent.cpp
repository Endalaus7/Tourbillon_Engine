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

TourBillon::PipelineData* TourBillon::PipelinePtr::loadData()
{

	return dynamic_cast<PipelineData*>(assetdata);
}

void TourBillon::PipelinePtr::releaseData()
{
}

TourBillon::SubPassData* TourBillon::SubpassPtr::loadData()
{
	return dynamic_cast<SubPassData*>(assetdata);
}

void TourBillon::SubpassPtr::releaseData()
{
}
TourBillon::PassData* TourBillon::PassPtr::loadData()
{
	return dynamic_cast<PassData*>(assetdata);
}
void TourBillon::PassPtr::releaseData()
{

}