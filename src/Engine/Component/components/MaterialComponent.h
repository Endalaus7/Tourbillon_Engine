#pragma once

#include "Component.h"
#include "ComponentData.h"

#include "ColorComponent.h"

#include "Assets.h"

//所有与几何相关的组件

namespace TourBillon
{
	class RHIImage;
	class RHIDeviceMemory;

	struct Texture 
	{
		STATIC_PROPERTY_CLASS_BEGIN(Texture)
			STATIC_PROPERTY_DEF(ReflectPath, imagePath)
			STATIC_PROPERTY_DEF(uint32_t, width)
			STATIC_PROPERTY_DEF(uint32_t, height)
			STATIC_PROPERTY_DEF(uint32_t, imageSize)
			STATIC_PROPERTY_DEF(uint32_t, texChannels)


		STATIC_PROPERTY_CLASS_END()
	public:
		RHIImage* image_buffer;
		RHIDeviceMemory* buffer_memory;
	};
	struct Shader {
		STATIC_PROPERTY_CLASS_BEGIN(Shader)
			STATIC_PROPERTY_DEF(ReflectPath, vertShaderPath)
			STATIC_PROPERTY_DEF(ReflectPath, fragShaderPath)

		STATIC_PROPERTY_CLASS_END()
	};
	
	struct Material:public AssetsData
	{
		STATIC_PROPERTY_CLASS_BEGIN(Material)
			STATIC_PROPERTY_DEF(ColorRGBA, color)
			STATIC_PROPERTY_DEF(Texture, MainTexture)
			STATIC_PROPERTY_DEF(Shader, shaders)
			//...
		STATIC_PROPERTY_CLASS_END()
	public:
	};
}