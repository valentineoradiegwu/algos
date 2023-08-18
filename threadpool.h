#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <future>
#include <condition_variable>

template<typename R>
class ThreadPool
{
public:
	using TaskType = std::packaged_task<R()>;
};
