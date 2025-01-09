#pragma once

#include "Component.h"
#include "ComponentData.h"

#include "ColorComponent.h"

#include "Assets.h"

//�����뼸����ص����

namespace TourBillon
{
	class RHIImage;
	class RHIDeviceMemory;
	class RHISampler;

	struct TextureData
	{
		STATIC_PROPERTY_CLASS_BEGIN()
			STATIC_PROPERTY_DEF(int, width)
			STATIC_PROPERTY_DEF(int, height)
			STATIC_PROPERTY_DEF(int, texChannels)


		STATIC_PROPERTY_CLASS_END()
	public:

		RHIImage* image_buffer;
		RHIDeviceMemory* buffer_memory;
		RHISampler* sampler;
	};

	struct TexturePtr :public Assets<TextureData>
	{
		~TexturePtr()override {}
		virtual TextureData* loadData()override;
		virtual void releaseData()override;
	};

}