#pragma once

//每种type注释其携带void* data类型

namespace TourBillon {

	namespace Events
	{
		enum EventType
		{
			LOAD_IMAGE,//TextureShared*
			LOAD_IMAGE_FINISHED,//TextureShared*
			LOAD_MODEL,//GeometryShared*
			LOAD_MODEL_FINISHED,


			KEY_CHANGE,//entity id
			MOUSE_MOVE,//entity id
		};
	}
}