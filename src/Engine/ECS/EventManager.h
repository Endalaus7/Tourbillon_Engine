#pragma once

#include "ECSCommon.h"
#include "ECSTypes.h"
#include "event.hpp"

namespace TourBillon
{
	class EventManager {
	public:
		void AddListener(Events::EventType type, std::function<void(const CEvent&)> callback);
		void SendEvent(Events::EventType type, void* data, size_t datasize);
	private:
		std::unordered_map<Events::EventType, CEvent> m_events;
	};
}