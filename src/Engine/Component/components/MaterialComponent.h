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
	class RHISampler;

	struct Texture
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
		RHISampler* sampler;
	};

	struct TextureShared :public Assets<Texture>
	{
		~TextureShared()override {}
		virtual Texture* loadData()override;
		virtual void releaseData()override;
	};

	struct PipelineData 
	{
		STATIC_PROPERTY_CLASS_BEGIN(PipelineData)
			STATIC_PROPERTY_DEF(ReflectPath, vertShaderPath)
			STATIC_PROPERTY_DEF(ReflectPath, fragShaderPath)

		STATIC_PROPERTY_CLASS_END()
	};

	struct PipelinePtr :public Assets<PipelineData>
	{
		~PipelinePtr()override {}
		virtual PipelineData* loadData()override;
		virtual void releaseData()override;
	};
	
	struct SubPassData
	{
		STATIC_PROPERTY_CLASS_BEGIN(SubPassData)
			STATIC_PROPERTY_DEF(PipelinePtr, bindPipeline)
			STATIC_PROPERTY_DEF(ReflectPath, fragShaderPath)

		STATIC_PROPERTY_CLASS_END()
	};
	struct SubpassPtr :public Assets<SubPassData>
	{
		~SubpassPtr()override {}
		virtual SubPassData* loadData()override;
		virtual void releaseData()override;
	};
	struct PassData 
	{
		STATIC_PROPERTY_CLASS_BEGIN(Pass)
			STATIC_PROPERTY_DEF_ARRAY(SubPassData, subpasses)

		STATIC_PROPERTY_CLASS_END()
	};

	struct PassPtr :public Assets<PassData>
	{
		~PassPtr()override {}
		virtual PassData* loadData()override;
		virtual void releaseData()override;
	};

	struct Material:public Component
	{
		STATIC_PROPERTY_CLASS_BEGIN(Material)
			STATIC_PROPERTY_DEF(ColorRGBA, basecolor)
			STATIC_PROPERTY_DEF(TextureShared, MainTexture)
			STATIC_PROPERTY_DEF_ARRAY(PassPtr, shaders)
			//...
		STATIC_PROPERTY_CLASS_END()
	public:
		
	};

	
}