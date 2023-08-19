#pragma once
#include <type_traits>
#include <new>
#include <utility>
#include <memory>
#include <stdexcept>
/*
* One might ask why dont we just use a std::array? Well, a std::array default initialises all its elements unless you use
* Aggregate initialiazation. Also, it is not possible to destruct an element in the array and construct a new one in its place.
* Finally the std::array is at full size from the off and we are unable to provide incremental growth up to capacity.
*/

namespace val::utils
{
	template <typename T, size_t CAP>
	class static_vector
	{
	public:
		static_assert(CAP >= 1);
		using iterator = T*;
		using const_iterator = const T*;

		static_vector() = default;
		static_vector(const static_vector&);
		static_vector(static_vector&&) noexcept;

		size_t capacity() const { return CAP; }
		size_t size() const { return size_; }
		bool empty() const { return size_ == 0; }

		T& back();
		const T& back() const;

		T& front();
		const T& front() const;

		void pop_back();
		void push_back(const T&);

		const T& operator[](std::size_t pos) const;
		T& operator[](std::size_t pos);

		template <typename ...PARAMS>
		void emplace_back(PARAMS&&... args);

		iterator begin() { return std::launder(reinterpret_cast<T*>(data_)); }
		const_iterator cbegin() const { return std::launder(reinterpret_cast<const T*>(data_)); }

		iterator end() { return std::launder(reinterpret_cast<T*>(data_ + size_)); }
		const_iterator cend() const { return std::launder(reinterpret_cast<const T*>(data_ + size_)); }
		~static_vector();
	private:
		std::aligned_storage_t<sizeof(T), alignof(T)> data_[CAP];
		size_t size_ = 0;
	};

	template <typename T, size_t CAP>
	static_vector<T, CAP>::static_vector(const static_vector& rhs)
		:size_{ rhs.size_ }
	{
		for (auto size = 0; size < size_; ++size)
		{
			//No need for launder here as memory is uninitialised?
			::new(&data_[size]) T{ rhs[size] };
		}
	}

	template <typename T, size_t CAP>
	static_vector<T, CAP>::static_vector(static_vector&& rhs) noexcept
		:size_{ rhs.size_}
	{
		std::uninitialized_move(rhs.begin(), rhs.end(), begin());
		rhs.size_ = 0;
	}

	template <typename T, size_t CAP>
	T& static_vector<T, CAP>::front()
	{
		if (empty())
			throw std::out_of_range{ "out of range" };
		return *begin();
	}

	template <typename T, size_t CAP>
	const T& static_vector<T, CAP>::front() const
	{
		if (empty())
			throw std::out_of_range{ "out of range" };
		return *cbegin();
	}

	template <typename T, size_t CAP>
	T& static_vector<T, CAP>::back()
	{
		if (empty())
			throw std::out_of_range{"out of range"};
		return *(end() - 1);
	}

	template <typename T, size_t CAP>
	const T& static_vector<T, CAP>::back() const
	{
		if (empty())
			throw std::out_of_range{ "out of range" };
		return *(cend() - 1);
	}

	template <typename T, size_t CAP>
	void static_vector<T, CAP>::pop_back()
	{
		if (empty())
			throw std::out_of_range{};
		std::destroy_at(std::launder(reinterpret_cast<T*>(&data_[--size_])));
	}

	template <typename T, size_t CAP>
	void static_vector<T, CAP>::push_back(const T& item)
	{
		if (size_ >= CAP)
			throw std::bad_alloc{};
		::new(&data_[size_]) T{ item };
		++size_;
	}

	template <typename T, size_t CAP>
	const T& static_vector<T, CAP>::operator[](std::size_t pos) const
	{
		return *std::launder(reinterpret_cast<const T*>(&data_[pos]));
	}

	template <typename T, size_t CAP>
	T& static_vector<T, CAP>::operator[](std::size_t pos)
	{
		return *std::launder(reinterpret_cast<T*>(&data_[pos]));
	}

	template <typename T, size_t CAP>
	template <typename ...PARAMS>
	void static_vector<T, CAP>::emplace_back(PARAMS&&... args)
	{
		if (size_ >= CAP)
			throw std::bad_alloc{};

		::new(&data_[size_]) T(std::forward<PARAMS>(args)...);
		++size_;
	}

	template <typename T, size_t CAP>
	static_vector<T, CAP>::~static_vector()
	{
		for (std::size_t pos = 0; pos < size_; ++pos)
			std::destroy_at(std::launder(reinterpret_cast<T*>(&data_[pos])));
	}
}
