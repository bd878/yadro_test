#pragma once

#include <iostream>
#include <string>

#include "TimeFormat.hpp"

enum class EventID {
	ClientArrive = 1,
	ClientTakeFreeTable = 2,
	ClientWaitingInQueue = 3,
	ClientLeave = 4,
	ClientTimeout = 11,
	NextClientInQueue = 12,
	Error = 13,
};

inline std::istream& operator>>(std::istream& is, EventID& event_id)
{
	int id = 0;
	is >> id;
	event_id = static_cast<EventID>(id);
	return is;
}

inline std::ostream& operator<<(std::ostream& os, const EventID& event_id)
{
	int id = static_cast<int>(event_id);
	os << id;
	return os;
}

class EventParams
{
	TimeFormat m_time;
	EventID m_id;
	std::string m_error;
	std::string m_name;
	int m_table;

public:
	EventParams() = default;

	EventParams(
		EventID id,
		TimeFormat time,
		std::string error,
		std::string name,
		int table
	) : m_id(id),
		m_time(time),
		m_error(error),
		m_name(name),
		m_table(table)
	{
	}

	const TimeFormat& GetTime() const { return m_time; }
	EventID GetID() const { return m_id; }
	std::string GetName() const { return m_name; }
	int GetTable() const { return m_table; }
	std::string GetError() const { return m_error; }

	friend std::istream& operator>>(std::istream& is, EventParams& params);
};

std::istream& operator>>(std::istream& is, EventParams& params)
{
	is >> params.m_time >> params.m_id;
	switch (params.m_id) {
	case EventID::ClientArrive:
	case EventID::ClientWaitingInQueue:
	case EventID::ClientLeave:
	case EventID::ClientTimeout:
	case EventID::NextClientInQueue: {
		is >> params.m_name;
		break;
	}
	case EventID::ClientTakeFreeTable: {
		is >> params.m_table;
		break;
	}
	case EventID::Error: {
		is >> params.m_error;
		break;
	}
	default:
		break;
	}
	return is;
}