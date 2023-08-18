#include "memoryblock.h"
#include <algorithm>

MemoryBlock::MemoryBlock()
	:m_data{nullptr},
	m_size{0}
{
}

MemoryBlock::MemoryBlock(const MemoryBlock& rhs)
	:m_data{new int[rhs.m_size]},
	m_size{ rhs.m_size }
{
	std::copy(rhs.m_data, rhs.m_data + rhs.m_size, m_data);
}

MemoryBlock::MemoryBlock(MemoryBlock&& rhs)
	:m_data{rhs.m_data},
	m_size{rhs.m_size}
{
	rhs.m_data = nullptr;
	rhs.m_size = 0;
}

MemoryBlock& MemoryBlock::operator=(const MemoryBlock& rhs)
{
	if (this != &rhs)
	{
		auto old_data = m_data;
		m_data = new int[rhs.m_size];
		m_size = rhs.m_size;
		std::copy(rhs.m_data, rhs.m_data + rhs.m_size, m_data);
		delete[] old_data;
	}
	return *this;
}

MemoryBlock& MemoryBlock::operator=(MemoryBlock&& rhs)
{
	if (this != &rhs)
	{
		delete[] m_data;
		m_data = rhs.m_data;
		m_size = rhs.m_size;
		rhs.m_data = nullptr;
		rhs.m_size = 0;
	}
	return *this;
}

MemoryBlock::~MemoryBlock()
{
	delete[] m_data;
}