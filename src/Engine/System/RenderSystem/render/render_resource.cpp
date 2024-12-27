#include "render_resource.h"
#include "ECSManager.h"

#include "components/MaterialComponent.h"

void TourBillon::RenderSource::init(RenderSystem* render_system)
{
	m_render_system = render_system;
	ECSManager::Instance()->AddListener(Events::LOAD_IMAGE_FINISHED, METHOD_LISTENER(RenderSource::loadTexture));
}

void TourBillon::RenderSource::loadTexture(const CEvent& event)
{
	TextureShared* texturePtr = (TextureShared*)event.event_data;
	Texture* textureData = dynamic_cast<Texture*>(texturePtr->getData());
	if (!textureData)return;
	size_t imageSize = textureData->width * textureData->height * textureData->texChannels;
	m_render_system->m_rhi->createTextureImage(textureData, imageSize, textureData->width, textureData->height, textureData->texChannels, textureData->image_buffer, textureData->buffer_memory);

}
