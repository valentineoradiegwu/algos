#include "stackmax.h"
#include <algorithm>

/*
* An interesting follow on question came up. This solution duplicates the max until a new max is encounted
* which is redundant storage of data. We can solve this by recording a count of elements from max to next max on each push 
* and decrement on each pop. Once the count goes to 0, we delete that max and the new max will be at the back.
*/
int StackMax::top() const
{
	return m_stack.top().first;
}

int StackMax::max() const
{
	return m_stack.top().second;
}

void StackMax::pop()
{
	m_stack.pop();
}

void StackMax::push(int item)
{
	auto maximum = empty() ? item : std::max(item, max());
	m_stack.push({ item, maximum });
}
StackMax::StackMax(StackMax&& rhs)
	:m_stack(std::move(rhs.m_stack))
{

}