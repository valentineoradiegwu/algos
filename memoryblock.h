#pragma once
/*
Default ctor
copy ctor
move ctor
copy assignment
move assignment
dtor
*/
class MemoryBlock
{
public:
	MemoryBlock();
	MemoryBlock(const MemoryBlock& rhs);
	MemoryBlock(MemoryBlock&& rhs);
	MemoryBlock& operator=(const MemoryBlock& rh);
	MemoryBlock& operator=(MemoryBlock&& rh);
	~MemoryBlock();
private:
	int* m_data;
	size_t m_size;
};
