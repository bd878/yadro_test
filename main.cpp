#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <exception>

std::pair<std::string, std::string> SplitTwo(std::string& str, char delim)
{
	const size_t pos = str.find(delim);
	if (pos == std::string::npos) {
		return {str, ""};
	} else {
		return {str.substr(0, pos), str.substr(pos+1)};
	}
}

int ConvertStrToInt(const std::string& str)
{
	size_t pos;
	const int num = std::stoi(str, &pos);
	if (pos != str.length()) {
		std::stringstream error;
		error << "string " << str << " contains " << (str.length() - pos) << "trailing chars";
		throw std::invalid_argument(error.str());
	}
	return num;
}

struct TimeFormat
{
	int minutes;
	int hours;
};

std::istream& operator>>(std::istream& is, TimeFormat& tf)
{
	std::string str;
	is >> str;

	const auto [lhs, rhs] = SplitTwo(str, ':');
	tf.hours = ConvertStrToInt(lhs);
	tf.minutes = ConvertStrToInt(rhs);

	return is;
}

struct Input
{
	TimeFormat time_open;
	TimeFormat time_close;
	int tables_count;
	int hour_price;
};

std::istream& operator>>(std::istream& is, Input& in)
{
	is >> in.tables_count >> in.time_open >> in.time_close
		>> in.hour_price;

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
		std::cerr << "failed to open " << argv[1] << std::endl;
		exit(EXIT_FAILURE);
	}

	Input in;
	fin >> in;
	std::cout << in.time_open.hours << ":" << in.time_open.minutes << std::endl;

	return 0;
}