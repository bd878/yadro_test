#pragma once

#include <string>
#include <memory>
#include <iostream>

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

	virtual void Print(std::ostream& os) const
	{
		os << m_id << " " << m_time;
	}

	EventID GetID() const { return m_id; }
	const TimeFormat& GetTime() const { return m_time; }
};

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

	virtual void Print(std::ostream& os) const
	{
		Event::Print(os);
		os << " " << m_name;
	}
};

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

	virtual void Print(std::ostream& os) const
	{
		Event::Print(os);
		os << " " << m_error;
	}
};

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

	virtual void Print(std::ostream& os) const
	{
		Event::Print(os);
		os << " " << m_table;
	}
};
