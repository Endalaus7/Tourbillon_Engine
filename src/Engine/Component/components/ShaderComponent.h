#pragma once

#include "Component.h"
#include "ComponentData.h"
#include "TextureComponent.h"
#include "ColorComponent.h"

#include "Assets.h"

//�����뼸����ص����

namespace TourBillon
{
	
	struct Pass
	{
		STATIC_PROPERTY_CLASS_BEGIN()
			
		STATIC_PROPERTY_CLASS_END()
	};

	
	struct ShaderData
	{
		STATIC_PROPERTY_CLASS_BEGIN()
			STATIC_PROPERTY_DEF()
		STATIC_PROPERTY_CLASS_END()
	};
	//����glsl shader�����Ǵ�������pass�ĳ��󼯺���
	struct ShaderPtr : public Assets<ShaderData>
	{
		~ShaderPtr()override {}
		virtual ShaderData* loadData()override;
		virtual void releaseData()override;
	};
}