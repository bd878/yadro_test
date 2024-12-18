#pragma once

#include <memory>

#include "ComputerClass.hpp"
#include "Events.hpp"

class ComputerClassBuilder
{
	std::shared_ptr<ComputerTables> m_tables;
	std::shared_ptr<Clients> m_clients;
	std::shared_ptr<Events> m_events;

	TimeFormat m_time_open;
	TimeFormat m_time_close;

public:
	void AddTables(std::shared_ptr<ComputerTables> tables) { m_tables = tables; }
	void AddClients(std::shared_ptr<Clients> clients) { m_clients = clients; }
	void AddEvents(std::shared_ptr<Events> events) { m_events = events; }
	void AddTimeOpen(TimeFormat time_open) { m_time_open = time_open; }
	void AddTimeClose(TimeFormat time_close) { m_time_close = time_close; }

	std::unique_ptr<ComputerClass> BuildComputerClass()
	{
		return std::make_unique<ComputerClass>(
			m_tables,
			m_clients,
			m_events,
			m_time_open,
			m_time_close
		);
	}
};
