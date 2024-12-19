#pragma once

#include <utility>
#include <iostream>
#include <memory>
#include <string>
#include <exception>
#include <cstdio>

#include "TimeFormat.hpp"
#include "EventFactory.hpp"
#include "EventParams.hpp"
#include "Event.hpp"
#include "ComputerClassBuilder.hpp"
#include "ComputerClass.hpp"

class ProgramState
{
public:
	enum State {
		Init = 0,
		Horaires = 1,
		Price = 2,
		EventFeed = 3,
		Stop = 4
	};

private:
	State m_state = ProgramState::State::Init;

	TimeFormat m_time_open;
	TimeFormat m_time_close;
	int m_tables_count = 0;
	int m_price_for_hour = 0;

	std::unique_ptr<ComputerClassBuilder> m_builder;
	std::unique_ptr<ComputerClass> m_computer_class;

public:
	ProgramState(std::unique_ptr<ComputerClassBuilder> b)
		: m_builder(std::move(b)) {}

	void Change(std::string);
	void Finish();

	friend std::ostream& operator<<(std::ostream&, const ProgramState&);
};

void ProgramState::Change(std::string cmd_line)
{
	std::stringstream is(cmd_line);

	switch (m_state) {
		case ProgramState::State::Init: {
			int ret = std::sscanf(cmd_line.c_str(), "%d", &m_tables_count);
			if (ret == 0 || ret == EOF) {
				throw std::invalid_argument(cmd_line);
			}
			m_builder->AddClients(std::make_shared<Clients>(m_tables_count));
			m_builder->AddEvents(std::make_shared<Events>());
			m_state = ProgramState::State::Horaires;
			break;
		}
		case ProgramState::State::Horaires: {
			is >> m_time_open >> m_time_close;
			m_builder->AddTimeOpen(m_time_open);
			m_builder->AddTimeClose(m_time_close);
			m_state = ProgramState::State::Price;
			break;
		}
		case ProgramState::State::Price: {
			int ret = std::sscanf(cmd_line.c_str(), "%d", &m_price_for_hour);
			if (ret == 0 || ret == EOF) {
				throw std::invalid_argument(cmd_line);
			}
			m_builder->AddTables(std::make_shared<ComputerTables>(m_tables_count, m_price_for_hour));
			m_computer_class = std::move(m_builder->BuildComputerClass());
			m_state = ProgramState::State::EventFeed;
			break;
		}
		case ProgramState::State::EventFeed: {
			EventParams params;
			is >> params;
			auto event = TheEventFactory::Instance()->Create(params.GetID());
			event->Load(params);
			m_computer_class->HandleEvent(event);
			break;
		}
		default:
			break;
	}
}

void ProgramState::Finish()
{
	m_state = ProgramState::State::Stop;
	if (m_computer_class.get() != nullptr) {
		m_computer_class->Close();
	}
}

std::ostream& operator<<(std::ostream& os, const ProgramState& ps)
{
	os << *ps.m_computer_class.get() << "\n";
	return os;
}
