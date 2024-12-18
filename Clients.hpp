#pragma once

#include <iostream>
#include <queue>
#include <map>
#include <unordered_map>

class Clients
{
	std::unordered_map<std::string, int> m_tables;
	std::queue<std::string> m_queue;
	std::map<std::string, int> m_clients;

	int m_tables_count;

public:
	Clients() = default;

	Clients(int tables_count) : m_tables_count(tables_count) {}

	bool HasClient(std::string name) const
	{
		return m_clients.find(name) != m_clients.end();
	}

	void AddClient(std::string name)
	{
		m_clients[name] = 1;
	}

	void ClientLeave(std::string name)
	{
		ClientFreeTable(name);
		m_clients.erase(name);
	}

	int GetClientTable(std::string name)
	{
		auto it = m_tables.find(name);
		if (it == m_tables.end()) {
			return -1;
		}
		return it->second;
	}

	void ClientTakeTable(std::string name, int table)
	{
		if (HasClient(name)) {
			m_tables.insert({name, table});
		}
	}

	void ClientFreeTable(std::string name)
	{
		int table = GetClientTable(name);
		if (table != -1) {
			m_tables.erase(name);
		}
	}

	void AddClientToQueue(std::string name)
	{
		if (HasClient(name)) {
			m_queue.push(name);
		}
	}

	bool IsQueueFull()
	{
		return m_queue.size() >= m_tables_count;
	}

	std::string PopClientFromQueue()
	{
		auto result = m_queue.front();
		m_queue.pop();
		return result;
	}
};
