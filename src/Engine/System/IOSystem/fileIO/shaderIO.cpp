#include "shaderIO.h"
#include "ECSManager.h"

#include "components/MaterialComponent.h"

void TourBillon::shaderIO::init()
{
	//stbi_load();
	ECSManager::Instance()->AddListener(Events::LOAD_IMAGE, METHOD_LISTENER(shaderIO::loadShader));
}

void TourBillon::shaderIO::loadShader(const CEvent&)
{


}

