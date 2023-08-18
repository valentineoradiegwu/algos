#pragma once
#include <unordered_map>
#include <list>
#include <utility>

template <typename KEY, typename VALUE>
class LRUCache
{
public:
	using List = std::list<KEY>;
	using Map = std::unordered_map<KEY, std::pair<typename List::iterator, VALUE>>;
	using MapIter = typename Map::const_iterator;

	LRUCache(size_t capacity);
	LRUCache(const LRUCache&) = delete;
	LRUCache(LRUCache&&) = default;
	LRUCache& operator=(const LRUCache&) = delete;
	LRUCache& operator=(LRUCache&&) = default;

	bool erase(const KEY& key);
	bool insert(const std::pair<KEY, VALUE>& item);
	VALUE find(const KEY& key);

	auto size() const { return m_map.size(); }
	bool empty() const { return m_map.empty(); }
	size_t cap() const { return m_capacity; }
private:
	size_t m_capacity;
	List m_list;
	Map m_map;
};

template <typename KEY, typename VALUE>
LRUCache<KEY, VALUE>::LRUCache(size_t capacity)
	:m_capacity{capacity},
	m_list{},
	m_map{}
{}

template <typename KEY, typename VALUE>
bool LRUCache<KEY, VALUE>::insert(const std::pair<KEY, VALUE>& item)
{
	auto iter = m_map.find(item.first);
	if (iter != m_map.end())
	{
		m_list.splice(m_list.begin(), iter->second.first, m_list);
		return false;
	}

	if (size() < cap())
	{
		m_list.push_front(item.first);
		return m_map.insert({ item.first, {m_list.begin(), item.second} }).second;
	}

	//Got to erase least recently use to make way
	auto key = m_list.back();
	m_list.pop_back();
	m_map.erase(key);
	m_list.push_front(item.first);
	return m_map.insert({ item.first, {m_list.begin(), item.second} }).second;
}

template <typename KEY, typename VALUE>
VALUE LRUCache<KEY, VALUE>::find(const KEY& key)
{
	auto iter = m_map.find(key);
	if (iter == m_map.end())
		return VALUE{};
	m_list.splice(m_list.begin(), iter->second.first, m_list);
	return iter->second.second;
}
//using Map = std::unordered_map<KEY, std::pair<typename List::iterator, VALUE>>;
template <typename KEY, typename VALUE>
bool LRUCache<KEY, VALUE>::erase(const KEY& key)
{
	auto iter = m_map.find(key);
	if (iter == m_map.end())
		return false;
	m_list.erase(iter->second.first);
	m_map.erase(iter);
	return true;
}