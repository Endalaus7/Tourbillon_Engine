#pragma once

//每种type注释其携带void* data类型

namespace TourBillon {

	namespace Events
	{
		enum EventType
		{
			LOAD_IMAGE,//stbi_uc*
			LOAD_MODEL,


			KEY_CHANGE,//entity id
			MOUSE_MOVE,//entity id
		};
	}
}