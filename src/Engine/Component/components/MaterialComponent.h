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

	struct Texture :public AssetsData
	{
		STATIC_PROPERTY_CLASS_BEGIN(Texture)
			STATIC_PROPERTY_DEF(ReflectPath, imagePath)
			STATIC_PROPERTY_DEF(int, width)
			STATIC_PROPERTY_DEF(int, height)
			STATIC_PROPERTY_DEF(int, texChannels)


		STATIC_PROPERTY_CLASS_END()
	public:

		RHIImage* image_buffer;
		RHIDeviceMemory* buffer_memory;
	};

	struct TextureShared :public Assets
	{
		~TextureShared()override {}
		virtual Texture* loadData()override;
	};

	struct Shader {
		STATIC_PROPERTY_CLASS_BEGIN(Shader)
			STATIC_PROPERTY_DEF(ReflectPath, vertShaderPath)
			STATIC_PROPERTY_DEF(ReflectPath, fragShaderPath)

		STATIC_PROPERTY_CLASS_END()
	};
	
	struct Material:public Component
	{
		STATIC_PROPERTY_CLASS_BEGIN(Material)
			STATIC_PROPERTY_DEF(ColorRGBA, basecolor)
			STATIC_PROPERTY_DEF(TextureShared, MainTexture)
			STATIC_PROPERTY_DEF(Shader, shaders)
			//...
		STATIC_PROPERTY_CLASS_END()
	public:
		
	};

	
}