#pragma once
#include <atomic>
//Create a base class for control block. Why?
//create a subclass for deleter
//Why is the delete_resource in control block? the pointer is in shared_ptr though.

struct control_block
{
	virtual void delete_resource() = 0;
	virtual ~control_block() {};
	std::atomic<int> shared_count{ 1 };
	std::atomic<int> weak_count{ 0 };
};

template <typename T, typename Deleter>
struct control_block_impl : control_block
{
	control_block_impl(Deleter&& d)
		:deleter{std::move(d)}
	{}
	virtual void delete_resource() override
	{
		deleter();
	}
	Deleter deleter;
};

template <typename T>
class MySharedPtr
{
public:
	using DeleterType = control_block_impl<T>;
	MySharedPtr()
		:m_ptr{nullptr},
		m_ctrl{nullptr}
	{}
	template <Typename U, Typename Deleter>
	MySharedPtr(U* ptr, Deleter d)
		: m_ptr{ptr}
	{
		auto d_functor = [ld = std::move(d), ptr]{ d(ptr); };
		m_ctrl = new control_block_impl<decltype(d_functor)>{ std::move(d_functor) };
	}
	~MySharedPtr()
	{
		decrement_use_count();
	}

private:
	void increment_use_count()
	{
		++m_ctrl->shared_count;
	}
	void decrement_use_count()
	{
		if (!--m_ctrl->shared_count)
		{
			m_ctrl->delete_resource();
			delete m_ctrl;
		}
	}
	T* m_ptr;
	control_block* m_ctrl;
};