#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <exception>

#include "TimeFormat.hpp"
#include "EventParams.hpp"
#include "Event.hpp"
#include "Events.hpp"

class ComputerTables
{
	std::vector<std::string> table_to_client;

public:
	const bool HasFreeTable();
	const int GetFreeTable();
	const int GetClientOnTable(int table);

	void Take(int table, std::string client);
	void Untake(int table, std::string client);
};

class Clients
{
	std::unordered_map<std::string, int> client_to_table;

public:
	const bool HasClient(std::string client);
	const int GetClientTable(std::string client);

	void Take(int table, std::string client);
	void Untake(int table, std::string client);
};

class ComputerClass
{
	ComputerTables* m_tables;
	Clients* m_clients;
	Events* m_events;

public:
	ComputerClass(
		ComputerTables* tables,
		Clients* clients,
		Events* events
	) : m_tables(tables),
		m_clients(clients),
		m_events(events)
	{}

	~ComputerClass()
	{
		delete m_tables;
		delete m_clients;
		delete m_events;
	}

	void HandleEvent(Event&);
};

void HandleEvent(Event& event)
{}

class ComputerClassBuilder
{
	ComputerTables* m_tables;
	Clients* m_clients;
	Events* m_events;

public:

	void AddTables(ComputerTables* tables) { m_tables = tables; }
	void AddClients(Clients* clients) { m_clients = clients; }
	void AddEvents(Events* events) { m_events = events; }

	ComputerClass* BuildComputerClass()
	{
		return new ComputerClass(
			m_tables,
			m_clients,
			m_events
		);
	}
};

class ProgramState
{
	enum State {
		Init = 0,
		Horaires = 1,
		Price = 2,
		EventFeed = 3
	};

	State state = ProgramState::State::Init;

	TimeFormat m_time_open;
	TimeFormat m_time_close;
	int m_tables_count = 0;
	int m_price_for_hour = 0;

	ComputerClassBuilder* m_builder;
	ComputerClass* m_computer_class;

public:
	ProgramState(ComputerClassBuilder* b)
		: builder(b) {}

	~ProgramState() {
		delete m_builder;
		if (m_computer_class != nullptr) {
			delete m_computer_class;
		}
	}

	void Shift(std::string);
};

void Shift(std::string cmd_line)
{
	std::stringstream is(cmd_line);

	switch (state) {
		case ProgramState::State::Init: {
			is >> m_tables_count;
			m_builder->AddTables(new ComputerTables(m_tables_count));
			m_builder->AddClients(new Clients);
			state = ProgramState::State::Horaires;
			break;
		}
		case ProgramState::State::Horaires: {
			is >> m_time_open >> m_time_close;
			state = ProgramState::State::Price;
			break;
		}
		case ProgramState::State::Price: {
			is >> m_price_for_hour;
			m_computer_class = m_builder->BuildComputerClass();
			state = ProgramState::State::EventFeed;
			break;
		}
		case ProgramState::State::EventFeed: {
			Event event;
			EventParams params;
			is >> params;
			event.Load(params);
			m_computer_class->HandleEvent(event);
			break;
		}
		default:
			break;
	}
}

int main(int argc, char const *argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " test_file.txt\n";
		return 2;
	}

	std::fstream fin(argv[1]);

	if (!fin.is_open()) {
		std::cerr << "fail to open " << argv[1] << std::endl;
		std::exit(EXIT_FAILURE);
	}

	ProgramState state(new ComputerClassBuilder);

	std::string line;
	while (std::getline(fin, line) && !line.empty()) {
		try {
			state.Shift(line);
		} catch (std::exception& e) {
			std::cerr << "fail on line: " << line << "\n"
				<< ", error: " << e.what() << std::endl;
			std::exit(EXIT_FAILURE);
		}
	}

	std::cout << reader;

	return 0;
}