#include "imageIO.h"
#include "ECSManager.h"

#include "components/MaterialComponent.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void TourBillon::imageIO::init()
{
	//stbi_load();
	ECSManager::Instance()->AddListener(Events::LOAD_IMAGE, METHOD_LISTENER(imageIO::loadPicture));
}

void TourBillon::imageIO::loadPicture(const CEvent& event)
{
	TexturePtr* texturePtr = (TexturePtr*)event.event_data;
	auto path = texturePtr->getAssetPath();
	
	int width, height, texChannels;
	stbi_uc* pixel = stbi_load(path.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);

	if (!pixel)
	{
		LOG_WARNING("pixle load failed");
		return;
	}
		

	TextureData* loadingTexture = new TextureData;
	loadingTexture->width = width;
	loadingTexture->height = height;
	loadingTexture->texChannels = texChannels;
	texturePtr->setAssetData(loadingTexture);

	ECSManager::Instance()->SendEvent(Events::LOAD_IMAGE_FINISHED, texturePtr);

	stbi_image_free(pixel);
}
