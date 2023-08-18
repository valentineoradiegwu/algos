#pragma once
#include <map>

class CountIntervals
{
public:
	CountIntervals() = default;
	void add(int left, int right);
	int count() const { return m_count; };
private:
	std::map<int, std::pair<int, int>> m_map;
	int m_count;
};