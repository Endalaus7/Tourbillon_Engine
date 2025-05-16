#pragma once

//每种type注释其携带void* data类型

namespace TourBillon {

	namespace Events
	{
		enum EventType
		{
			LOAD_IMAGE,//TexturePtr*
			LOAD_IMAGE_FINISHED,//TexturePtr*
			RELEASE_IMAGE,//TexturePtr*

			LOAD_MODEL,//GeometryPtr*
			//LOAD_MODEL_FINISHED,

			LOAD_SHADER,//Shader*


			KEY_CHANGE,//entity id
			MOUSE_MOVE,//entity id

			LOAD_YAML,//ReflectPath path
		};
	}
}