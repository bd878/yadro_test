#pragma once

#include <istream>
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

class EventParams
{
	TimeFormat m_time;
	EventID m_id;
	std::string m_name;
	int m_table;

public:
	EventParams() = default;
	EventParams(
		TimeFormat time,
		EventID id,
		std::string name,
		int table
	) : 
	m_time(time),
	m_id(id),
	m_name(name),
	m_table(table)
	{}

	TimeFormat GetTime() const { return m_time; }
	EventID GetID() const { return m_id; }
	std::string GetName() const { return m_name; }
	int GetTable() const { return m_table; }

	friend std::istream& operator>>(std::istream& is, EventParams& params);
};

std::istream& operator>>(std::istream& is, EventParams& params)
{
	is >> params.m_time >> params.m_id >> params.m_name;
	switch (params.m_id) {
		case EventID::ClientTakeFreeTable:
			is >> params.m_table;
			break;
		default:
			break;
	}
	return is;
}