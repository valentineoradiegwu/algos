#include "memorybuffer.h"
#include <algorithm>

MemoryBuffer::MemoryBuffer(size_t size)
	:m_buffer{new int[size]},
	m_size{size}
{}

MemoryBuffer::MemoryBuffer(const MemoryBuffer& rhs)
	: m_buffer{new int[rhs.m_size]},
	m_size{ rhs.m_size }
{
	std::copy(rhs.m_buffer, rhs.m_buffer + rhs.m_size, m_buffer);
}

MemoryBuffer::MemoryBuffer(MemoryBuffer&& rhs)
	:m_buffer{ rhs.m_buffer },
	m_size{rhs.m_size}
{
	rhs.m_buffer = nullptr;
	rhs.m_size = 0;
}

//Is it exception safe? Now it is
MemoryBuffer& MemoryBuffer::operator=(const MemoryBuffer& rhs)
{
	if (this != &rhs)
	{
		auto tmp = m_buffer;
		m_size = rhs.m_size;
		m_buffer = new int[m_size];
		std::copy(rhs.m_buffer, rhs.m_buffer + rhs.m_size, m_buffer);
		delete[] tmp;
	}
	return *this;
}

//Should release resources in owning class or swap to moved object and let them delete when it goes out of scope?
MemoryBuffer& MemoryBuffer::operator=(MemoryBuffer&& rhs)
{
	if (this != &rhs)
	{
		auto tmp = m_buffer;
		m_buffer = rhs.m_buffer;
		m_size = rhs.m_size;
		rhs.m_buffer = nullptr;
		rhs.m_size = 0;
		delete[] tmp;
	}
	return *this;
}

MemoryBuffer::~MemoryBuffer()
{
	delete[] m_buffer;
}