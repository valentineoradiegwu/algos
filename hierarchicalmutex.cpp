#include "hierarchicalmutex.h"

thread_local int HierarchicalMutex::currentLevel = INT_MAX;

HierarchicalMutex::HierarchicalMutex(int level)
	:m_level{ level },
	m_prevLevel{0}
{}

void HierarchicalMutex::lock()
{
	IsValid();
	m_mutex.lock();
	UpdateLevels();
}
void HierarchicalMutex::unlock()
{
	currentLevel = m_prevLevel;
	m_mutex.unlock();
}
bool HierarchicalMutex::try_lock()
{
	IsValid();
	if (!m_mutex.try_lock())
		return false;
	UpdateLevels();
	return true;
}

bool HierarchicalMutex::IsValid()
{
	if (m_level >= currentLevel)
		throw std::logic_error{ "Cannot acquire a lock greater than or equal to level" };
	return true;
}
void HierarchicalMutex::UpdateLevels()
{
	m_prevLevel = currentLevel;
	currentLevel = m_level;
}