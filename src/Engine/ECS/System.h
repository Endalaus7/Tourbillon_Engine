#pragma once
#include <set>
#include "ECSCommon.h"
//ÿ��system��¼����component��entity�Ĺ�ϵ

namespace TourBillon
{
	struct SystemInitInfo
	{
		virtual ~SystemInitInfo() {}
		void* init_message;
		//std::shared_ptr<RenderWindow> window_system;
		//std::shared_ptr<DebugDrawManager> debugdraw_manager;
	};
	class System
	{
	public:
		virtual void initialize(SystemInitInfo* init_info) = 0;

		//void receiveEvent();

		std::set<Entity> mEntities;
	protected:
	};
}