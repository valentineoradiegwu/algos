#pragma once

#include <new>
#include <atomic>
#include <optional>
#include <type_traits>

//The relaxed memory ordering semantics only guarantee that the operations on the same atomic type inside 
//the same thread cannot be reordered, and this guarantee is called modification order consistency
namespace val::utils
{
	template <typename T>
	concept Insertable = std::is_trivially_destructible_v<T>;

	template <Insertable T, size_t CAP>
	class spsc_overwrite
	{
		static_assert(CAP > 0, "Capacity must be greater than 0");
		static_assert((CAP & (CAP - 1)) == 0, "Capacity must be a power of 2");
	public:
		spsc_overwrite() = default;
		spsc_overwrite(const spsc_overwrite&) = delete;
		spsc_overwrite(spsc_overwrite&&) = delete;
		spsc_overwrite& operator=(const spsc_overwrite&) = delete;
		spsc_overwrite& operator=(spsc_overwrite&&) = delete;

		void push(const T& item) noexcept(std::is_nothrow_copy_constructible_v<T>)
		{
			static_assert(std::is_copy_constructible_v<T>, "T must be copy constructible");
			emplace(item);
		}


		template <typename... Args>
		void emplace(Args&&...args) noexcept(std::is_nothrow_constructible_v<T, Args&&...>)
		{
			const uint64_t writeIdx = writeIdx_.load(std::memory_order_relaxed);
			::new(&data_[mask(writeIdx)]) T{ std::forward<Args>(args)... };
			writeIdx_.store(writeIdx + 1, std::memory_order_release);
		}

		std::optional<T> pop() noexcept
		{
			std::optional<T> copy;

			if (empty())
			{
				return copy;
			}

			uint64_t writeIdx, diff;
			uint64_t readIdx = readIdx_.load(std::memory_order_acquire);

			do
			{
				copy = *(reinterpret_cast<T*>(&data_[mask(readIdx)]));
				std::atomic_signal_fence(std::memory_order_acq_rel); //Prevent reordering
				writeIdx = writeIdx_.load(std::memory_order_acquire);
				diff = writeIdx - readIdx;
				//If wrapped around, adjust the readIdx for next iteration to the oldest item which is now at writeIdx - CAP
				readIdx = (diff > CAP) ? writeIdx - CAP : readIdx;
			} while (diff > CAP);

			readIdx_.store(readIdx + 1, std::memory_order_release);
			return copy;
		}

		[[nodiscard]] size_t capacity() const noexcept { return CAP; }

		[[nodiscard]] size_t size() const noexcept 
		{ 
			return writeIdx_.load(std::memory_order_acquire) - readIdx_.load(std::memory_order_acquire);
		}

		[[nodiscard]] bool empty() const noexcept
		{
			return writeIdx_.load(std::memory_order_acquire) == readIdx_.load(std::memory_order_acquire);
		}

		[[nodiscard]] bool full() const noexcept
		{
			return size() == capacity();
		}

	private:
#ifdef __cpp_lib_hardware_interference_size
		static constexpr size_t CACHE_LINE_SIZE = std::hardware_destructive_interference_size;
#else
		static constexpr size_t CACHE_LINE_SIZE = 64;
#endif
		uint64_t mask(const uint64_t idx) const noexcept { return idx & (CAP - 1); }
		//To prevent false sharing
		alignas(CACHE_LINE_SIZE) std::atomic<uint64_t> writeIdx_ = { 0 };
		alignas(CACHE_LINE_SIZE) std::atomic<uint64_t> readIdx_ = { 0 };
		std::aligned_storage_t<sizeof(T), alignof(T)> data_[CAP];
	};
}
