#pragma once
#include <thread>
#include <stdexcept>

class thread_guard
{
public:
    thread_guard(std::thread&& th)
        :m_th{ std::move(th) }
    {
        if (!m_th.joinable())
            throw std::logic_error{ "No thread" };
    }

    thread_guard(thread_guard&& lhs)
        :m_th{ std::move(lhs.m_th) }
    {
        if (!m_th.joinable())
            throw std::logic_error{ "No thread" };
    }

    ~thread_guard()
    {
        if (m_th.joinable())
            m_th.join();
    }

    thread_guard(const thread_guard&) = delete;
    thread_guard& operator=(const thread_guard&) = delete;
private:
    std::thread m_th;
};
