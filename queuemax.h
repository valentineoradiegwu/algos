#pragma once
#include <queue>
#include <deque>

/*
* I dont know what to say. Somehow i was able to work this out and code it but its so complex i dont believe i just did it.
* Imagine you had the following in a queue [70,80,60,20,30, 80]. By the time we add item 60, we realise 20 can only be 
* dequeued before 60 therefore 20 can never be the max. By the way we insert 80, we realise 60 can never be the max so we eject it
* iteratively until we find an element greater than or equal to us.
*/
template <typename T>
class QueueWithMax
{
public:
	QueueWithMax = default;
	void pop();
	T front() const { return queue.front(); };
	T max() const { return deque.front(); };
	void push(const T& item);
	bool empty() const { return queue.empty(); };
private:
	std::queue<T> queue;
	std::deque<T> deque;
};

template <typename T>
void QueueWithMax<T>::pop()
{
	const auto head = queue.front();
	queue.pop();
	if (head == deque.front())
		deque.pop_front();
}

template <typename T>
void QueueWithMax<T>::push(const T& item)
{
	queue.push(item);
	while(!deque.empty() && deque.back() < item)
	{ 
		deque.pop_back();
	}
	deque.push_back(item);
}
