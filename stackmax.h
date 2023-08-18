#pragma once
#include <stack>
class StackMax
{
public:
	StackMax() = default;
	StackMax(const StackMax& rhs) = default;
	StackMax(StackMax&& rhs);
	int top() const;
	int max() const;
	void pop();
	void push(int item);
	bool empty() const
	{
		return m_stack.empty();
	}
private:
	std::stack<std::pair<int, int>> m_stack;
};