#pragma once

#include <string>
#include <memory>

#include "TimeFormat.hpp"
#include "EventParams.hpp"

class Event
{
	EventID m_id;
	TimeFormat m_time;

public:
	Event() = default;

	virtual ~Event() {}

	virtual void Load(EventParams const& params)
	{
		m_id = params.GetID();
		m_time = params.GetTime();
	}

	EventID GetID() const { return m_id; }
	const TimeFormat& GetTime() const { return m_time; }

	friend std::ostream& operator<<(std::ostream&, const Event&);
};

std::ostream& operator<<(std::ostream& os, const Event& ev)
{
	os << ev.m_id << " " << ev.m_time;
	return os;
}

class ClientEvent : public Event
{
	std::string m_name;

public:
	ClientEvent() : Event() {}

	virtual void Load(EventParams const& params)
	{
		Event::Load(params);
		m_name = params.GetName();
	}

	friend std::ostream& operator<<(std::ostream&, const ClientEvent&);
};

std::ostream& operator<<(std::ostream& os, const ClientEvent& ev)
{
	os << static_cast<const Event&>(ev) << " " << ev.m_name;
	return os;
}

class ErrorEvent : public Event
{
	std::string m_error;

public:
	ErrorEvent() : Event() {}

	virtual void Load(EventParams const& params)
	{
		Event::Load(params);
		m_error = params.GetError();
	}

	friend std::ostream& operator<<(std::ostream&, const ErrorEvent&);
};

std::ostream& operator<<(std::ostream& os, const ErrorEvent& ev)
{
	os << static_cast<const Event&>(ev) << " " << ev.m_error;
	return os;
}

class TableEvent : public Event
{
	int m_table;
public:
	TableEvent() : Event() {}

	virtual void Load(EventParams const& params)
	{
		Event::Load(params);
		m_table = params.GetTable();
	}

	friend std::ostream& operator<<(std::ostream&, const TableEvent&);
};

std::ostream& operator<<(std::ostream& os, const TableEvent& ev)
{
	os << static_cast<const Event&>(ev) << " " << ev.m_table;

	return os;
}