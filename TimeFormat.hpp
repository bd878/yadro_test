#pragma once

#include <iostream>
#include <string>
#include <exception>
#include <sstream>

struct TimeFormat
{
	int hours;
	int minutes;

	TimeFormat() :
		hours(0),
		minutes(0)
	{}
};

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

std::istream& operator>>(std::istream& is, TimeFormat& tf)
{
	std::string str;
	is >> str;

	const auto [lhs, rhs] = SplitTwo(str, ':');
	tf.hours = ConvertStrToInt(lhs);
	tf.minutes = ConvertStrToInt(rhs);

	return is;
}

std::ostream& operator<<(std::ostream& os, TimeFormat& tf)
{
	/* how much bytes would have been written */
	const int n_bytes = snprintf(nullptr, 0, "%02d:%02d", tf.hours, tf.minutes);
	std::string out;
	out.resize(n_bytes+1);
	snprintf(&out[0], out.size(), "%02d:%02d", tf.hours, tf.minutes);
	os << out;
	return os;
}