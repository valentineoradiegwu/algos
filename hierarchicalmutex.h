#pragma once
#include <mutex>

class HierarchicalMutex
{
public:
	HierarchicalMutex(int level);
	HierarchicalMutex(const HierarchicalMutex&)=delete;
	HierarchicalMutex(HierarchicalMutex&&)=delete;
	HierarchicalMutex& operator=(const HierarchicalMutex&)=delete;
	HierarchicalMutex& operator=(HierarchicalMutex&&)=delete;
	void lock();
	void unlock();
	bool try_lock();
private:
	const int m_level;
	int m_prevLevel;
	std::mutex m_mutex;
	bool IsValid();
	void UpdateLevels();
	static thread_local int currentLevel;
};
