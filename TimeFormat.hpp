#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <exception>
#include <sstream>

const int MINUTES_IN_HOURS = 60;

struct TimeFormat
{
	int m_hours;
	int m_minutes;

	TimeFormat() :
		m_hours(0),
		m_minutes(0)
	{}

	TimeFormat(int minutes)
	{
		auto div = std::div(minutes, MINUTES_IN_HOURS);
		m_minutes = div.rem;
		m_hours = div.quot;
	}

	TimeFormat(TimeFormat& tf)
	{
		m_hours = tf.m_hours;
		m_minutes = tf.m_minutes;
	}

	int ToMinutes() const
	{
		int minutes = m_hours*MINUTES_IN_HOURS;
		return minutes + m_minutes;
	}

	TimeFormat operator -(const TimeFormat& rhs)
	{
		int minutes = this->ToMinutes() - rhs.ToMinutes();
		return TimeFormat(minutes);
	}
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
	tf.m_hours = ConvertStrToInt(lhs);
	tf.m_minutes = ConvertStrToInt(rhs);

	return is;
}

std::ostream& operator<<(std::ostream& os, const TimeFormat& tf)
{
	/* how much bytes would have been written */
	const int n_bytes = snprintf(nullptr, 0, "%02d:%02d", tf.m_hours, tf.m_minutes);
	std::string out;
	out.resize(n_bytes+1);
	snprintf(&out[0], out.size(), "%02d:%02d", tf.m_hours, tf.m_minutes);
	os << out;
	return os;
}