#pragma once

#include <string>
#include <ostream>
#include <algorithm>
#include <vector>

#include "TimeFormat.hpp"

class ComputerTables
{
	struct TableStat
	{
		int id;
		int minutes_total;
		bool is_table_free;
		int revenue;
		TimeFormat busy_start_time;

		TableStat(int _id)
			: id(_id), minutes_total(0), is_table_free(true), revenue(0)
		{}
	};

	int m_tables_count;
	int m_hour_price;
	std::vector<std::shared_ptr<TableStat>> m_tables;

public:
	ComputerTables(int count, int price)
		: m_tables_count(count), m_hour_price(price)
	{
		m_tables.resize(count);
		for (int i = 0; i < m_tables.size(); i++) {
			m_tables[i] = std::make_shared<TableStat>(i+1);
		}
	}

	int GetAnyFreeTable() const
	{
		auto it = std::find_if(m_tables.begin(), m_tables.end(), [](const auto& it) {
			return it->is_table_free;
		});
		if (it == m_tables.end()) {
			return -1;
		}
		return std::distance(m_tables.begin(), it);
	}

	bool IsTableBusy(int table_id) const
	{
		return !m_tables.at(table_id-1)->is_table_free;
	}

	void TakeTable(int table_id, TimeFormat time)
	{
		auto table = m_tables.at(table_id-1);
		if (table->is_table_free) {
			table->is_table_free = false;
			table->busy_start_time = time;
		}
	}

	void FreeTable(int table_id, TimeFormat time)
	{
		auto table = m_tables.at(table_id-1);
		if (!table->is_table_free) {
			table->is_table_free = true;
			auto busy_time = time - table->busy_start_time;
			table->minutes_total += busy_time.ToMinutes();
			table->revenue += (busy_time.GetHours() * m_hour_price);
			if (busy_time.GetMinutes() > 0) {
				table->revenue += m_hour_price;
			}
			table->busy_start_time.Reset();
		}
	}

	friend std::ostream& operator<<(std::ostream&, ComputerTables const&);
};

std::ostream& operator<<(std::ostream& os, ComputerTables const& tables)
{
	std::for_each(tables.m_tables.begin(), tables.m_tables.end(), [&os](const auto& table) {
		TimeFormat total_time(table->minutes_total);
		os << table->id << " " << table->revenue << " " << total_time << "\n";
	});
	return os;
}