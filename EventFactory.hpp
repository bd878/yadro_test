#pragma once

#include <iostream>
#include <memory>

#include "EventParams.hpp"
#include "Event.hpp"

class EventFactory
{
	static EventFactory* p_instance;
public:
	static EventFactory* Instance()
	{
		if (p_instance == nullptr) {
			p_instance = new EventFactory();
		}
		return p_instance;
	}

	std::shared_ptr<Event> Create(EventID id) const
	{
		switch (id) {
		case EventID::ClientTakeFreeTable:
			return std::make_shared<TableEvent>();
			break;
		case EventID::ClientArrive:
		case EventID::ClientWaitingInQueue:
		case EventID::ClientLeave:
		case EventID::ClientTimeout:
		case EventID::NextClientInQueue: {
			return std::make_shared<ClientEvent>();
			break;
		}
		case EventID::Error:
			return std::make_shared<ErrorEvent>();
			break;
		default:
			return std::make_shared<Event>();
		}
	}

	~EventFactory()
	{
		if (p_instance != nullptr) {
			delete p_instance;
		}
	}
};

EventFactory* EventFactory::p_instance = nullptr;

typedef EventFactory TheEventFactory;
