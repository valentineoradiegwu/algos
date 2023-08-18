#pragma once

class MemoryBuffer
{
public:
	MemoryBuffer(size_t size);
	MemoryBuffer(const MemoryBuffer& rhs);
	MemoryBuffer(MemoryBuffer&& rhs);
	MemoryBuffer& operator=(const MemoryBuffer& rhs);
	MemoryBuffer& operator=(MemoryBuffer&& rhs);
	~MemoryBuffer();
private:
	int* m_buffer;
	size_t m_size;
};
