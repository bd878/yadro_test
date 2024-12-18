#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <fstream>
#include <exception>

#include "ProgramState.hpp"
#include "ComputerClassBuilder.hpp"

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

	ProgramState state(std::make_unique<ComputerClassBuilder>());

	std::string cmd_line;
	while (std::getline(fin, cmd_line) && !cmd_line.empty()) {
		try {
			state.Change(cmd_line);
		} catch (std::exception& e) {
			std::cerr << "fail on line: " << cmd_line << "\n"
				<< ", error: " << e.what() << std::endl;
			std::exit(EXIT_FAILURE);
		}
	}

	state.Finish();

	std::cout << state;

	return 0;
}	