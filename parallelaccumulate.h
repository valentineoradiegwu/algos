#pragma once
#include <vector>

class ParallelAccumulate
{
public:
	static const int MIN_BATCH_SIZE;
	ParallelAccumulate(const std::vector<int>& input, int init)
		:m_data{ input }, m_init{init}
	{}
	int Accummulate() const;
private:
	const int m_init;
	const std::vector<int> m_data;
	using Iter = decltype(m_data.begin());
	static void initial_function(Iter begin, Iter end, int& result);
};
