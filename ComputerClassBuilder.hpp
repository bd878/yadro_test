#pragma once

#include <memory>

#include "ComputerClass.hpp"
#include "Events.hpp"

class ComputerClassBuilder
{
	std::shared_ptr<ComputerTables> m_tables;
	std::shared_ptr<Clients> m_clients;
	std::shared_ptr<Events> m_events;

public:
	void AddTables(std::shared_ptr<ComputerTables> tables) { m_tables = tables; }
	void AddClients(std::shared_ptr<Clients> clients) { m_clients = clients; }
	void AddEvents(std::shared_ptr<Events> events) { m_events = events; }

	std::unique_ptr<ComputerClass> BuildComputerClass()
	{
		return std::make_unique<ComputerClass>(
			m_tables,
			m_clients,
			m_events
		);
	}
};
