#pragma once

#include <new>
#include <atomic>
#include <type_traits>

namespace val::utils
{
	template <typename T>
	concept Insertable = std::is_trivially_destructible_v<T>;

	template <typename T, size_t CAP>
	class spsc_overwrite
	{
	public:
		void push(const T& item)
		{
			writeIdx_.fetch_add(1);

			auto writeIdx = writeIdx_.load(std::memory_order_relaxed);
			auto validity = validities_[writeIdx % CAP].load(std::memory_order_acquire);

			while (validity != 0 && (writeIdx - validity != CAP))
			{
				writeIdx_.fetch_add(1);

				writeIdx = writeIdx_.load(std::memory_order_relaxed);
				validity = validities_[writeIdx % CAP].load(std::memory_order_acquire);
			}
			::new(&data_[writeIdx % CAP]) T{ item };
			validities_[writeIdx % CAP].store(writeIdx, std::memory_order_release);
		}

		bool pop(T& ptr)
		{
			auto readIdx = readIdx_.load(std::memory_order_relaxed);
			auto expected_validity = readIdx;

			//I only want to update the validity slot to claim it from being overwritten by writer if it is equal to my readIdx
			while (!validities_[readIdx % CAP].compare_exchange_strong(expected_validity, readIdx + 1, std::memory_order_release, std::memory_order_relaxed))
			{
				if (expected_validity == 0)
				{
					//Validity for the slot is 0 so the RQ is empty. Note CAS updated expected_validity with what is in the validities slot.
					return false;
				}
				//Tail is not equal to validity
				//Advance tail but sometimes u might want to advance it by big jumps if the writer has far overtaken the reader by more than 2 times
				const auto diff = expected_validity - readIdx;
				if (diff > CAP)
					readIdx_.store(expected_validity - CAP, std::memory_order_relaxed);
				else
					readIdx_.fetch_add(1);

				readIdx = readIdx_.load(std::memory_order_relaxed);
				expected_validity = readIdx;
			}
			ptr = *(reinterpret_cast<T*>(&data_[readIdx % CAP]));
			validities_[readIdx % CAP].store(0, std::memory_order_release); //flag slot as available for write.
			readIdx_.fetch_add(1);
			return true;
		}
	
	private:
#ifdef __cpp_lib_hardware_interference_size
		static constexpr size_t CACHE_LINE_SIZE = std::hardware_destructive_interference_size;
#else
		static constexpr size_t CACHE_LINE_SIZE = 64;
#endif
		alignas(CACHE_LINE_SIZE) std::atomic<uint64_t> writeIdx_ = { 0 };
		alignas(CACHE_LINE_SIZE) std::atomic<uint64_t> readIdx_ = { 1 };
		std::aligned_storage_t<sizeof(T), alignof(T)> data_[CAP];
		std::array<std::atomic<uint64_t>, CAP> validities_{};
	};
}
