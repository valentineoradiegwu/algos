#include <algorithm>
#include "countintervals.h"
/*
* The solution is pretty straightforward in the end
* Search for the interval bigger than what we are inserting.
*/
void CountIntervals::add(int left, int right)
{
    auto iter = m_map.upper_bound(left);
    if (iter != m_map.begin())
    {
        --iter;
        if (iter->second.second < left)
        {
            ++iter;
        }
    }
    while (iter != m_map.end() && right >= iter->second.first)
    {
        left = std::min(left, iter->second.first);
        right = std::max(right, iter->second.second);
        m_count -= iter->second.second - iter->second.first + 1;
        iter = m_map.erase(iter);
    }
    m_map.insert({ left, {left, right} });
    m_count += right - left + 1;
}