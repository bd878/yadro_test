#pragma once

#include <ostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "Event.hpp"
#include "Events.hpp"
#include "EventFactory.hpp"

class ComputerTables
{
	struct TableStat
	{
		int id;
		int minutes_total;
		std::string client_name;
		bool is_table_free;
		TimeFormat busy_start_time;

		TableStat(int _id)
		: id(_id), minutes_total(0)
		{}
	};

	std::vector<TableStat*> m_tables;

public:
	ComputerTables(int count)
	{
		m_tables.resize(count);
		for (int i = 0; i < m_tables.size(); i++) {
			m_tables[i] = new TableStat(i);
		}
	}

	int GetAnyFreeTable() const
	{
		auto it = std::find_if(m_tables.begin(), m_tables.end(), [](const auto& it) {
			return it->is_table_free;
		});
		if (it == m_tables.end()) {
			return -1;
		}
		return std::distance(m_tables.begin(), it);
	}

	bool IsTableBusy(int table_id) const
	{
		return !m_tables.at(table_id)->is_table_free;
	}

	void TakeTable(int table_id, std::string client_name, TimeFormat time)
	{
		auto table = m_tables.at(table_id);
		if (table->is_table_free) {
			table->is_table_free = false;
			table->client_name = client_name;
			table->busy_start_time = time;
		}
	}

	void FreeTable(int table_id, TimeFormat time)
	{
		auto table = m_tables.at(table_id);
		if (!table->is_table_free) {
			table->is_table_free = true;
			table->client_name = "";
			auto busy_timme = time - table->busy_start_time;
			table->minutes_total += busy_timme.ToMinutes();
			table->busy_start_time.Reset();
		}
	}
};

class Clients
{
	std::unordered_map<std::string, int> m_client_to_table;

public:
	Clients() = default;

	bool HasClient(std::string name) const
	{
		auto it = m_client_to_table.find(name);
		return m_client_to_table.end() != it;
	}

	void ClientTakeTable(std::string name, int table)
	{
		m_client_to_table.insert({name, table});
	}
};

class ComputerClass
{
	std::shared_ptr<ComputerTables> m_tables;
	std::shared_ptr<Clients> m_clients;
	std::shared_ptr<Events> m_events;

	TimeFormat m_time_open;
	TimeFormat m_time_close;

public:
	ComputerClass(
		std::shared_ptr<ComputerTables> tables,
		std::shared_ptr<Clients> clients,
		std::shared_ptr<Events> events,
		TimeFormat time_open,
		TimeFormat time_close
	) : m_tables(tables),
		m_clients(clients),
		m_events(events),
		m_time_open(time_open),
		m_time_close(time_close)
	{}

	void HandleEvent(Event*);

	friend std::ostream& operator<<(std::ostream&, ComputerClass const&);

private:
	void handleClientArrive(ClientEvent*);
	void handleClientTakeTable(TableEvent*);
};

void ComputerClass::HandleEvent(Event* ev)
{
	m_events->InsertAfter(m_events->GetLast(), ev);

	switch (ev->GetID()) {
	case EventID::ClientArrive: {
		handleClientArrive(dynamic_cast<ClientEvent*>(ev));
		break;
	}
	case EventID::ClientTakeFreeTable: {
		handleClientTakeTable(dynamic_cast<TableEvent*>(ev));
		break;
	}
	case EventID::Error:
		break;
	}
}

void ComputerClass::handleClientArrive(ClientEvent* ev)
{
	if (m_clients->HasClient(ev->GetClientName())) {
		auto event = TheEventFactory::Instance()->Create(EventID::Error);
		event->Load(EventParams(
			EventID::Error, ev->GetTime(), "YouShallNotPass", 0, 0
		));
		this->HandleEvent(event);
		return;
	}

	if (ev->GetTime() < m_time_open || ev->GetTime() > m_time_close) {
		auto event = TheEventFactory::Instance()->Create(EventID::Error);
		event->Load(EventParams(
			EventID::Error, ev->GetTime(), "NotOpenYet", "", 0
		));
		this->HandleEvent(event);
		return;
	}
}

void ComputerClass::handleClientTakeTable(TableEvent*)
{
}

std::ostream& operator<<(std::ostream& os, ComputerClass const& cs)
{
	cs.m_events->Traverse([&os](const Event* ev) {
		ev->Print(os);
		os << "\n";
	});
	return os;
}
