#pragma once

#include <memory>
#include <ostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "Event.hpp"
#include "Events.hpp"

class ComputerTables
{
	struct TableStat
	{
		int id;
		int revenue;
		int time_total;
		bool is_table_free;
		TimeFormat busy_at;

		TableStat(int _id)
		: id(_id), revenue(0), time_total(0)
		{}
	};

	std::vector<std::string> m_table_to_client;
	std::vector<std::unique_ptr<TableStat>> m_tables;

public:
	ComputerTables(int count)
	{
		m_table_to_client = std::vector<std::string>(count, "");
		m_tables.resize(count);
		for (int i = 0; i < m_tables.size(); i++) {
			m_tables[i] = std::make_unique<TableStat>(i);
		}
	}

	int GetAnyFreeTable() const
	{
		auto it = std::find(m_table_to_client.begin(), m_table_to_client.end(), "");
		if (it == m_table_to_client.end()) {
			return -1;
		}
		return std::distance(m_table_to_client.begin(), it);
	}

	bool IsTableBusy(int table_id) const
	{
		return m_table_to_client.at(table_id) != "";
	}

	void TakeTable(int table_id, std::string client_name)
	{
		m_table_to_client[table_id] = client_name;
	}

	void FreeTable(int table_id)
	{
		m_table_to_client[table_id] = "";
	}
};

class Clients
{
	std::unordered_map<std::string, int> m_client_to_table;

public:
	Clients() = default;
};

class ComputerClass
{
	std::shared_ptr<ComputerTables> m_tables;
	std::shared_ptr<Clients> m_clients;
	std::shared_ptr<Events> m_events;

public:
	ComputerClass(
		std::shared_ptr<ComputerTables> tables,
		std::shared_ptr<Clients> clients,
		std::shared_ptr<Events> events
	) : m_tables(tables),
		m_clients(clients),
		m_events(events)
	{}

	void HandleEvent(Event*);

	friend std::ostream& operator<<(std::ostream&, ComputerClass const&);
};

void ComputerClass::HandleEvent(Event* ev)
{
	m_events->InsertAfter(m_events->GetHead(), ev);
}

std::ostream& operator<<(std::ostream& os, ComputerClass const& cs)
{
	cs.m_events->Traverse([&os](const Event* ev) {
		ev->Print(os);
		os << "\n";
	});
	return os;
}
