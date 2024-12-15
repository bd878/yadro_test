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

enum class InputState {
	Init = 0,
	Horaires = 1,
	Price = 2,
	EventFeed = 3
};

struct Input
{
	InputState state = InputState::Init;
	Events* events;

	TimeFormat time_open;
	TimeFormat time_close;
	int tables_count = 0;
	int price_for_hour = 0;

	Input(Events* ev) : events(ev) {}

	~Input() {
		delete events;
	}
};

std::istream& operator>>(std::istream& is, Input& in)
{
	switch (in.state) {
		case InputState::Init: {
			is >> in.tables_count;
			in.state = InputState::Horaires;
			break;
		}
		case InputState::Horaires: {
			is >> in.time_open >> in.time_close;
			in.state = InputState::Price;
			break;
		}
		case InputState::Price: {
			is >> in.price_for_hour;
			in.state = InputState::EventFeed;
			break;
		}
		case InputState::EventFeed: {
			Event event;
			EventParams params;
			is >> params;
			event.Load(params);
			in.events->InsertAfter(in.events->GetLast(), event);
			break;
		}
		default:
			break;
	}
	return is;
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

	Input input_state(new Events);

	std::string line;
	while (std::getline(fin, line) && !line.empty()) {
		std::stringstream line_stream(line);

		try {
			line_stream >> input_state;
		} catch (std::exception& e) {
			std::cerr << "fail on line: " << line << "\n"
				<< ", error: " << e.what() << std::endl;
			std::exit(EXIT_FAILURE);
		}
	}

	return 0;
}