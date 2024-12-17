#pragma once

#include <iostream>

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

	Event* Create(EventID id) const
	{
		switch (id) {
		case EventID::ClientTakeFreeTable:
			return new TableEvent();
			break;
		case EventID::ClientArrive:
		case EventID::ClientWaitingInQueue:
		case EventID::ClientLeave:
		case EventID::ClientTimeout:
		case EventID::NextClientInQueue: {
			return new ClientEvent();
			break;
		}
		case EventID::Error:
			return new ErrorEvent();
			break;
		default:
			return new Event();
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
