#pragma once
#include <vector>
#include <algorithm>

template <typename T>
class CircularQueue
{
public:
	CircularQueue(int initial_capacity = 8);
	bool empty() const { return size == 0; };
	bool full() const { return size == buffer.size(); };
	size_t size() const { return size; };
	const T front() const { return buffer[readidx]; };
	T front() { return buffer[readidx]; };
	void pop();
	void push(const T& item);
private:
	std::vector<T> buffer;
	int writeidx;
	int readidx;
	size_t size;
};

template <typename T>
CircularQueue<T>::CircularQueue(int initial_capacity)
	:buffer(initial_capacity),
	writeidx{0},
	readidx{0},
	size{0}
{}

template <typename T>
void CircularQueue<T>::pop()
{
	if (!empty())
	{
		readidx = (readidx + 1) % buffer.size();
		--size;
	}
}

template <typename T>
void CircularQueue<T>::push(const T& item)
{
	if (full())
	{
		//Normalise and update pointers and Extend underlying vector
		//assert that readidx == writeidx
		//RotateLeft(buffer, writeidx);
		std::rotate(buffer.begin(), buffer.begin() + writeidx, buffer.end())
		readidx = 0;
		writeidx = size;
		buffer.resize(size * 2);
	}
	else
	{
		buffer[writeidx] = item;
		writeidx = (writeidx + 1) % buffer.size();
	}
}