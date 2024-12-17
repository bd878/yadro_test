cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(yadro_test)

add_executable(yadro_test
	main.cpp
	ProgramState.hpp
	ComputerClass.hpp
	ComputerClassBuilder.hpp
	EventFactory.hpp
	Events.hpp
	TimeFormat.hpp
	Event.hpp
	EventParams.hpp
	)

set_target_properties(yadro_test
	PROPERTIES
		CXX_STANDARD 20
		CXX_EXTENSIONS OFF
		CXX_STANDARD_REQUIRED OFF
	)