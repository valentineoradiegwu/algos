#pragma once

#include <new>
#include <atomic>
#include <optional>
#include <type_traits>
#include "scoped_exit.h"


/*
* This buffer overwrites the oldest item when it is full and the consumer cant keep up.
* A buffer which overwrites the oldest data is hard to get right without a lock. The canonical solution
* of such a buffer is for the writer to increment the read index upon the detection of a full buffer so that the reader
* consumes from the oldest and the writer can overwrite. All easy with a lock of course. To create a lock free impl,
* these are the challenges.
* 1. The typical spsc lock free impl expects only the producer thread to update the write index and the consumer thread
*    to update the read index. This would be violated since the producer has to update the read index when full.
* 2. Upon incrementing the read index by the writer, the reader could load this atomic read index to a local variable,
*    get preempted by the scheduler, and by the time it is rescheduled, it is operating with a stale read index that has been advanced by writer.
* 3. It is almost impossible to ensure that a location overwritten by the producer is not being simultaneous read by the
*    consumer.
* We need an extra field to determine if a slot is valid for reading or for writing and the following uses an atomic uint64_t
* per slot.
* 1. The field is called validity and one exist per slot on the queue. The options were to either embed this inside the message
*    or have a parallel array of these fields. I went for the latter for cache reasons as i want the data to be close to each other
*    without being interspersed by the validity field.
* 2. It starts off as 0.
* 3. Before a writer updates a slot, validity MUST be 0 (slot is empty or has been read) or a multiple of capacity + index.
*    For a capacity of 15, index 1 is valid if the field contains 1, 16, 31, 46 etc. This indicates the writer is overwriting the slot.
* 4. Before a reader reads a slot, it checks if its index is equal to validity. For example if the read index is 16 (logically 1 in a
*    15 cap queue i.e. 16%15) and the validity is 16, then the reader is reading the oldest value. The reader has to claim the slot
*    by atomically doing a CAS and writing 16 + 1 = 17. 17 breaks the A writer validity described in (3) and avoid overwriting slot
*    the slot and move to next one. Once reader has  finished reading, it sets validity to 0 thereby flagging the slot as available
*    for writing in next loop around. If the readers index is not equal to the index in validity, then the slot has been overwritten
*    and reader has to search for oldest slot. Sometimes the writer may have done a number of loops over the reader, and we have to adjust
*    the reader accordingly to then start searching for oldest. The oldest slot is the one whose validity is equal to the index of reader.
*/


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

		std::optional<T> pop() noexcept(std::is_nothrow_copy_constructible_v<T>)
		{
			auto readIdx = readIdx_ % CAP;
			auto expected_validity = readIdx_;

			//I only want to update the validity slot to claim it from being overwritten by writer if it is equal to my readIdx
			while (!validities_[readIdx].compare_exchange_strong(expected_validity, readIdx_ + 1, std::memory_order_release, std::memory_order_relaxed))
			{
				if (expected_validity == 0)
				{
					//Validity for the slot is 0 so the RQ is empty. Note CAS updated expected_validity with what is in the validities slot.
					return std::nullopt;
				}
				//Tail is not equal to validity
				//Advance tail by 1 but sometimes u might want to advance it by big jumps if the writer has far overtaken the reader by more than 2 times
				const auto diff = expected_validity - readIdx_;
				if (diff > CAP)
				{
					readIdx_ = expected_validity - CAP;
				}
				else
				{
					++readIdx_;
				}

				readIdx = readIdx_ % CAP;
				expected_validity = readIdx_;
			}

			++readIdx_;
			//flag slot as available for write after normal return or exception
			auto update = make_scoped_action([&]() { validities_[readIdx].store(0, std::memory_order_release); });
			std::optional<T> ret = *(reinterpret_cast<T*>(&data_[readIdx]));
			return ret;
		}

		[[nodiscard]] size_t capacity() const noexcept { return CAP; }

		[[nodiscard]] bool empty() const noexcept
		{
			return validities_[readIdx_ % CAP].load(std::memory_order_relaxed) == 0;
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
