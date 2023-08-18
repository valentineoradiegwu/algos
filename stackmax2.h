#pragma once
#include <stack>

class StackMaxO1
{
public:
	StackMaxO1() = default;
	auto size() const { return m_stack.size(); }
	bool empty() const { return m_stack.empty(); }
	int max() const { return m_max; }
	void push(int item)
	{
		if (m_stack.empty())
		{
			m_stack.push(item);
			m_max = item;
		}
		else
		{
			if (item > m_max)
			{
				m_stack.push(2 * item - m_max);
				m_max = item;
			}
			else
				m_stack.push(item);
		}
	}
	int top() const
	{
		const int top = m_stack.top();
		return top > m_max ? m_max : top;
	}
	void pop()
	{
		const int top = m_stack.top();
		m_stack.pop();
		if (top > m_max)
			m_max = m_max * 2 - top;
	}
private:
	std::stack<int> m_stack;
	int m_max;
};
