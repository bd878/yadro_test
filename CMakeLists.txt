cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(yadro_test)

add_executable(yadro_test main.cpp)

set_target_properties(yadro_test
	PROPERTIES
		CXX_STANDARD 17
		CXX_EXTENSIONS OFF
		CXX_STANDARD_REQUIRED OFF
	)