#pragma once
#include <stack>
#include <mutex>
#include <shared_mutex>

#include <memory>

/*
* The copy and move ctors and assignments are interesting.
* Note that any number of threads might be accessing either of the objects
* And note the difference between copy and assignment and the number of objects that need to be locked.
* Note you need to make mutex mutable so you can call lock even on a const object.
*/
template <typename T>
class ThreadSafeStack
{
public:
	using StackType = std::stack<T>;
	ThreadSafeStack();
	ThreadSafeStack(const ThreadSafeStack&);
	ThreadSafeStack(ThreadSafeStack&&);
	ThreadSafeStack& operator=(const ThreadSafeStack&);
	ThreadSafeStack& operator=(ThreadSafeStack&&);
	void push(const T& item);
	void pop(T& dest);
	std::shared_ptr<T> pop();
	bool empty() const 
	{ 
		std::lock_guard lg{ m_mutex };
		return m_stack.empty(); 
	};
	typename StackType::size_type size() const 
	{ 
		std::lock_guard lg{ m_mutex };
		return m_stack.size(); 
	}; //can deduce return type with auto
private:
	StackType m_stack;
	mutable std::mutex m_mutex;
};

template <typename T>
ThreadSafeStack<T>::ThreadSafeStack()
	:m_stack{},
	m_mutex{}
{}

template <typename T>
ThreadSafeStack<T>::ThreadSafeStack(const ThreadSafeStack& rhs)
	: m_stack{},
	m_mutex{}
{
	std::shared_lock<std::mutex> lg{ rhs.m_mutex };
	m_stack = rhs.m_stack;
}

template <typename T>
ThreadSafeStack<T>::ThreadSafeStack(ThreadSafeStack&& rhs)
	: m_stack{},
	m_mutex{}
{
	std::lock_guard<std::mutex> lg{ rhs.m_mutex };
	m_stack = std::move(rhs.m_stack);
}

template <typename T>
void ThreadSafeStack<T>::push(const T& item)
{
	std::lock_guard<std::mutex> lg{ m_mutex };
	m_stack.push(item);
}

template <typename T>
void ThreadSafeStack<T>::pop(T& dest)
{
	std::lock_guard<std::mutex> lg{ m_mutex };
	dest = m_stack.top();
	m_stack.pop();
}

template <typename T>
std::shared_ptr<T> ThreadSafeStack<T>::pop()
{
	std::lock_guard lg{ m_mutex };
	auto out = std::make_shared<T>(m_stack.top());
	m_stack.pop();
	return out;
}

template <typename T>
ThreadSafeStack<T>& ThreadSafeStack<T>::operator=(const ThreadSafeStack& rhs)
{
	if (this != &rhs)
	{
		std::unique_lock<std::mutex> me{ m_mutex, std::defer_lock };
		std::shared_lock<std::mutex> other{ rhs.m_mutex, std::defer_lock };
		std::lock(me, other);
		m_stack = rhs.m_stack;
	}
	return *this;
}

template <typename T>
ThreadSafeStack<T>& ThreadSafeStack<T>::operator=(ThreadSafeStack&& rhs)
{
	if (this != &rhs)
	{
		std::lock(m_mutex, rhs.m_mutex);
		std::lock_guard<std::mutex> me{ m_mutex, std::adopt_lock };
		std::lock_guard<std::mutex> me{ rhs.m_mutex, std::adopt_lock };
		m_stack = std::move(rhs.m_stack);
	}
	return *this;
}