#pragma once
#include <stack>
#include <stdexcept>

template <typename T>
class QueueWithStacks
{
public:
	QueueWithStacks() = default;
	void pop();
	void push(const T& item);
	T& front();
	const T& front() const;
	bool empty() const { return enqueue.empty() && dequeue.empty(); };
	auto size() const { return enqueue.size() + dequeue.size(); }
private:
	struct ReadEmptyException : std::logic_error
	{
		virtual const char* what() const
		{
			return "Queue is empty";
		}
	};
	std::stack<T> enqueue;
	std::stack<T> dequeue;
};

template <typename T>
void QueueWithStacks<T>::pop()
{
	if (!empty())
		throw ReadEmptyException{};

	if (dequeue.empty())
	{
		while (!enqueue.empty())
		{
			const auto item = enqueue.top();
			enqueue.pop();
			dequeue.push(item);
		}
	}
	dequeue.pop();
}

template <typename T>
void QueueWithStacks<T>::push(const T& item)
{
	enqueue.push(item);
}

template <typename T>
const T& QueueWithStacks<T>::front() const
{
	if (empty())
		throw ReadEmptyException{};

	if(dequeue.empty())
	{
		while (!enqueue.empty())
		{
			const auto item = enqueue.top();
			enqueue.pop();
			dequeue.push(item);
		}
	}
	return dequeue.top();
}

template <typename T>
T& QueueWithStacks<T>::front()
{
	return const_cast<T&>(const_cast<const QueueWithStacks<T>*>(this)->front());
}