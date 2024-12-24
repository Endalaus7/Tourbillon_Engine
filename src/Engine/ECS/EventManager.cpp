#include "EventManager.h"



void TourBillon::EventManager::AddListener(Events::EventType type, std::function<void(const CEvent&)> callback)
{
	m_events[type].addCallback(callback);
}

void TourBillon::EventManager::SendEvent(Events::EventType type, void* data, size_t datasize)
{
	auto eventitr = m_events.find(type);
	if (eventitr == m_events.end())
		return;
	eventitr->second.event_data = data;
	eventitr->second.dataSize = datasize;
	eventitr->second.trigger();
}
