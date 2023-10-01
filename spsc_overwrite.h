#pragma once

#include <new>
#include <atomic>
#include <type_traits>


//The relaxed memory ordering semantics only guarantee that the operations on the same atomic type inside 
//the same thread cannot be reordered, and this guarantee is called modification order consistency
namespace val::utils
{
	template <typename T>
	concept Insertable = std::is_trivially_destructible_v<T>;

	template <typename T, size_t CAP>
	class spsc_overwrite
	{
	public:
		void push(const T& item) noexcept(std::is_nothrow_copy_constructible_v<T>)
		{
			static_assert(std::is_copy_constructible_v<T>, "T must be copy constructible");
			emplace(item);
		}

		template <typename... Args>
		void emplace(Args&&...args) noexcept(std::is_nothrow_constructible_v<T, Args&&...>)
		{
			static_assert(std::is_constructible_v<T, Args&&...>, "T must be constructible with Args&&...");

			uint64_t writeIdx;
			uint64_t validity;

			do
			{
				++writeIdx_;
				writeIdx = writeIdx_ % CAP;
				validity = validities_[writeIdx].load(std::memory_order_acquire);
			} while (validity && (writeIdx != validity % CAP));

			::new(&data_[writeIdx]) T{ std::forward<Args>(args)... };
			validities_[writeIdx].store(writeIdx_, std::memory_order_release);
		}

		bool pop(T& ptr)
		{
			auto readIdx = readIdx_ % CAP;
			auto expected_validity = readIdx_;

			//I only want to update the validity slot to claim it from being overwritten by writer if it is equal to my readIdx
			while (!validities_[readIdx].compare_exchange_strong(expected_validity, readIdx_ + 1, std::memory_order_release, std::memory_order_relaxed))
			{
				if (expected_validity == 0)
				{
					//Validity for the slot is 0 so the RQ is empty. Note CAS updated expected_validity with what is in the validities slot.
					return false;
				}
				//Tail is not equal to validity
				//Advance tail but sometimes u might want to advance it by big jumps if the writer has far overtaken the reader by more than 2 times
				const auto diff = expected_validity - readIdx_;
				if (diff > CAP)
				{
					readIdx_ += (expected_validity - CAP);
				}
				else
				{
					++readIdx_;
				}

				readIdx = readIdx_ % CAP;
				expected_validity = readIdx_;
			}

			ptr = *(reinterpret_cast<T*>(&data_[readIdx]));
			validities_[readIdx].store(0, std::memory_order_release); //flag slot as available for write.
			++readIdx_;
			return true;
		}
	
	private:
#ifdef __cpp_lib_hardware_interference_size
		static constexpr size_t CACHE_LINE_SIZE = std::hardware_destructive_interference_size;
#else
		static constexpr size_t CACHE_LINE_SIZE = 64;
#endif
		//To prevent false sharing
		alignas(CACHE_LINE_SIZE) uint64_t writeIdx_ = { 0 };
		alignas(CACHE_LINE_SIZE) uint64_t readIdx_ = { 1 };
		std::aligned_storage_t<sizeof(T), alignof(T)> data_[CAP];
		std::array<std::atomic<uint64_t>, CAP> validities_{};
	};
}
