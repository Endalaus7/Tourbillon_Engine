#pragma once
#include <set>
#include "ECSCommon.h"
//每个system记录所有component和entity的关系

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