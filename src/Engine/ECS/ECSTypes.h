#pragma once

//ÿ��typeע����Я��void* data����

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