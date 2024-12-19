#pragma once

#include <ostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <unordered_map>

#include "ComputerTables.hpp"
#include "Clients.hpp"
#include "Event.hpp"
#include "Events.hpp"
#include "EventFactory.hpp"

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

	void HandleEvent(std::shared_ptr<Event>);

	void Close();

	friend std::ostream& operator<<(std::ostream&, ComputerClass const&);

private:
	void handleClientArrive(std::shared_ptr<ClientEvent>);
	void handleClientTakeTable(std::shared_ptr<TableEvent>);
	void handleClientWaiting(std::shared_ptr<ClientEvent>);
	void handleClientLeave(std::shared_ptr<ClientEvent>);
	void handleClientTimeout(std::shared_ptr<ClientEvent>);
};

void ComputerClass::Close()
{
	m_clients->Traverse([this](std::pair<const std::string, int> p) {
		auto event = TheEventFactory::Instance()->Create(EventID::ClientTimeout);
		event->Load(EventParams(
			EventID::ClientTimeout, m_time_close, "", p.first, 0
		));
		HandleEvent(event);
	});
}

void ComputerClass::HandleEvent(std::shared_ptr<Event> ev)
{
	m_events->InsertAfter(m_events->GetLast(), ev);

	switch (ev->GetID()) {
	case EventID::ClientArrive:
		handleClientArrive(std::dynamic_pointer_cast<ClientEvent>(ev));
		break;
	case EventID::ClientTakeFreeTable:
		handleClientTakeTable(std::dynamic_pointer_cast<TableEvent>(ev));
		break;
	case EventID::ClientWaitingInQueue:
		handleClientWaiting(std::dynamic_pointer_cast<ClientEvent>(ev));
		break;
	case EventID::ClientLeave:
		handleClientLeave(std::dynamic_pointer_cast<ClientEvent>(ev));
		break;
	case EventID::ClientTimeout:
		handleClientTimeout(std::dynamic_pointer_cast<ClientEvent>(ev));
		break;
	case EventID::Error:
		break;
	}
}

void ComputerClass::handleClientArrive(std::shared_ptr<ClientEvent> ev)
{
	if (m_clients->HasClient(ev->GetClientName())) {
		auto event = TheEventFactory::Instance()->Create(EventID::Error);
		event->Load(EventParams(
			EventID::Error, ev->GetTime(), "YouShallNotPass", "", 0
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

	m_clients->AddClient(ev->GetClientName());
}

void ComputerClass::handleClientTakeTable(std::shared_ptr<TableEvent> ev)
{
	if (!m_clients->HasClient(ev->GetClientName())) {
		auto event = TheEventFactory::Instance()->Create(EventID::Error);
		event->Load(EventParams(
			EventID::Error, ev->GetTime(), "ClientUnknown", "", 0
		));
		this->HandleEvent(event);
		return;
	}

	if (m_tables->IsTableBusy(ev->GetTableID())) {
		auto event = TheEventFactory::Instance()->Create(EventID::Error);
		event->Load(EventParams(
			EventID::Error, ev->GetTime(), "PlaceIsBusy", "", 0
		));
		this->HandleEvent(event);
		return;
	}

	int current_table = m_clients->GetClientTable(ev->GetClientName());
	if (current_table != -1) {
		m_tables->FreeTable(current_table, ev->GetTime());
	}
	m_clients->ClientTakeTable(ev->GetClientName(), ev->GetTableID());
	m_tables->TakeTable(ev->GetTableID(), ev->GetTime());
}

void ComputerClass::handleClientWaiting(std::shared_ptr<ClientEvent> ev)
{
	if (m_tables->GetAnyFreeTable() != -1) {
		auto event = TheEventFactory::Instance()->Create(EventID::Error);
		event->Load(EventParams(
			EventID::Error, ev->GetTime(), "ICanWaitNoLonger!", "", 0
		));
		this->HandleEvent(event);
		return;
	}

	if (m_clients->IsQueueFull()) {
		auto event = TheEventFactory::Instance()->Create(EventID::ClientTimeout);
		event->Load(EventParams(
			EventID::ClientTimeout, ev->GetTime(), "", ev->GetClientName(), 0
		));
		this->HandleEvent(event);
		return;
	}

	m_clients->AddClientToQueue(ev->GetClientName());
}

void ComputerClass::handleClientLeave(std::shared_ptr<ClientEvent> ev)
{
	if (!m_clients->HasClient(ev->GetClientName())) {
		auto event = TheEventFactory::Instance()->Create(EventID::Error);
		event->Load(EventParams(
			EventID::Error, ev->GetTime(), "ClientUnknown", ev->GetClientName(), 0
		));
		this->HandleEvent(event);
		return;
	}

	int table = m_clients->GetClientTable(ev->GetClientName());
	if (table != -1) {
		m_clients->ClientLeave(ev->GetClientName());
		m_tables->FreeTable(table, ev->GetTime());
		if (m_clients->IsQueueEmpty()) {
			return;
		}

		std::string client = m_clients->PopClientFromQueue();
		m_clients->ClientTakeTable(client, table);
		m_tables->TakeTable(table, ev->GetTime());

		auto event = TheEventFactory::Instance()->Create(EventID::NextClientInQueue);
		event->Load(EventParams(
			EventID::NextClientInQueue, ev->GetTime(), "", client, table
		));
		this->HandleEvent(event);
		return;
	}
}

void ComputerClass::handleClientTimeout(std::shared_ptr<ClientEvent> ev)
{
	if (!m_clients->HasClient(ev->GetClientName())) {
		return;
	}

	int table = m_clients->GetClientTable(ev->GetClientName());
	if (table != -1) {
		m_clients->ClientLeave(ev->GetClientName());
		m_tables->FreeTable(table, ev->GetTime());
	}
}

std::ostream& operator<<(std::ostream& os, ComputerClass const& cs)
{
	os << cs.m_time_open << "\n";
	cs.m_events->Traverse([&os](const std::shared_ptr<Event> ev) {
		ev->Print(os);
		os << "\n";
	});
	os << cs.m_time_close << "\n"
		<< *cs.m_tables.get() << "\n";
	return os;
}
