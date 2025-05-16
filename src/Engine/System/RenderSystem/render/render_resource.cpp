#include "render_resource.h"
#include "ECSManager.h"

#include "components/MaterialComponent.h"

void TourBillon::RenderSourceManager::init(RenderSystem* render_system)
{
	m_render_system = render_system;
	ECSManager::Instance()->AddListener(Events::LOAD_IMAGE_FINISHED, METHOD_LISTENER(RenderSourceManager::loadTexture));
	ECSManager::Instance()->AddListener(Events::RELEASE_IMAGE, METHOD_LISTENER(RenderSourceManager::releaseTexture));
}

void TourBillon::RenderSourceManager::loadTexture(const CEvent& event)
{
	TexturePtr* texturePtr = (TexturePtr*)event.event_data;
	TextureData* textureData = dynamic_cast<TextureData*>(texturePtr->getData());
	if (!textureData)return;
	size_t imageSize = textureData->width * textureData->height * 4;
	GET_VULKAN_RHI()->createTextureImage(textureData, imageSize, textureData->width, textureData->height, textureData->texChannels, textureData->image_buffer, textureData->buffer_memory);

	RHICreateTextureSamplerInfo sampler_create_info;
	sampler_create_info.addressmode = RHI_SAMPLER_ADDRESS_REPEAT;
	sampler_create_info.minFilter = RHI_SAMPLER_FILTER_LINEAR;
	sampler_create_info.magFilter = RHI_SAMPLER_FILTER_LINEAR;
	sampler_create_info.mipmapmode = RHI_MIPMAP_LINEAR;
	sampler_create_info.anisotropyEnable = true;
	m_render_system->m_rhi->createTextureSampler(sampler_create_info, textureData->sampler);

}

void TourBillon::RenderSourceManager::releaseTexture(const CEvent& event)
{
	TexturePtr* texturePtr = (TexturePtr*)event.event_data;
	TextureData* textureData = dynamic_cast<TextureData*>(texturePtr->getData());
	if (!textureData)return;
	m_render_system->m_rhi->destroySampler(textureData->sampler);
	m_render_system->m_rhi->destroyImage(textureData->image_buffer);
	m_render_system->m_rhi->destroyMemory(textureData->buffer_memory);
}
