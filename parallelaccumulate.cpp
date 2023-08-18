#include <thread>
#include <algorithm>
#include <numeric>
#include <iostream>
#include "parallelaccumulate.h"

const int ParallelAccumulate::MIN_BATCH_SIZE = 25;

int ParallelAccumulate::Accummulate() const
{
	if (m_data.empty())
		return m_init;

	const int hw_threads = std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 2;
	const int rem = m_data.size() % MIN_BATCH_SIZE;
	const int max_threads = m_data.size() / MIN_BATCH_SIZE + (rem ? 1 : 0);
	const int actual_threads = std::min(hw_threads, max_threads);
	const int batch_size = m_data.size() / actual_threads;

	std::vector<int> results(actual_threads);
	std::vector<std::thread> threads{};
	Iter start = m_data.begin();
	Iter end = m_data.begin() + batch_size;
	for (int i = 0; i < actual_threads - 1; ++i)
	{
		std::cout << "Launching thread " << (i + 1) << std::endl;
		threads.push_back(std::thread{ initial_function, start, end, std::ref(results[i])});
		start = end;
		end = start + batch_size;
	}

	initial_function(start, m_data.end(), std::ref(results[actual_threads - 1]));

	for (auto& th : threads)
		th.join();

	return std::accumulate(results.begin(), results.end(), m_init);
}

void ParallelAccumulate::initial_function(Iter begin, Iter end, int& result)
{
	result = std::accumulate(begin, end, 0);
}