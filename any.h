#pragma once

#include <memory>
#include <typeinfo>

namespace val::utils
{
	template <typename T>
	concept Copyable = std::is_copy_constructible_v<T>;

	class any
	{
		template <Copyable ValueType>
		friend const ValueType* any_cast(const any*) noexcept;

		template <Copyable ValueType>
		friend ValueType* any_cast(any*) noexcept;
	public:
		any() = default;

		any(const any& rhs)
			: instance_{ rhs.instance_ ? rhs.instance_->clone() : nullptr}
		{}

		any(any&& rhs)
			: instance_{std::move(rhs.instance_)}
		{}

		template <Copyable ValueType>
		any(ValueType&& rhs)
		{
			instance_ = std::make_unique<storage_impl<ValueType>>(std::forward<ValueType>(rhs));
		}

		any& operator=(const any& rhs)
		{
			if (this != &rhs && rhs.instance_)
			{
				instance_ = rhs.instance_->clone();
			}
			return *this;
		}

		any& operator=(any&& rhs)
		{
			if (this != &rhs)
			{
				instance_ = std::move(rhs.instance_);
			}
			return *this;
		}

		template <Copyable ValueType>
		any& operator=(ValueType&& rhs)
		{
			instance_ = std::make_unique<storage_impl<ValueType>>(std::forward<ValueType>(rhs));
			return *this;
		}

		bool has_value() const noexcept
		{
			return static_cast<bool>(instance_);
		}

		void reset()
		{
			instance_.reset();
		}

		const std::type_info& type() const noexcept
		{
			return instance_->type();
		}

		void swap(any& other)
		{
			std::swap(instance_, other.instance_);
		}

	private:
		struct base_storage_impl
		{
			virtual const std::type_info& type() const noexcept = 0;
			virtual std::unique_ptr<base_storage_impl> clone() const = 0;
			virtual ~base_storage_impl() = default;
		};

		template <Copyable ValueType>
		struct storage_impl : base_storage_impl
		{
			template <typename... ARGS>
			storage_impl(ARGS&& args)
				: value_type_{std::forward<ARGS>(args)...}
			{}

			virtual const std::type_info& type() const noexcept override
			{
				return typeid(ValueType);
			}

			virtual std::unique_ptr<base_storage_impl> clone() const override
			{
				return std::make_ptr<storage_impl<ValueType>(value_type_);
			}
			ValueType value_type_;

		};
		std::unique_ptr<base_storage_impl> instance_;
	};

	template <Copyable ValueType>
	const ValueType any_cast(const any& rhs)
	{
		auto ptr = any_cast<ValueType>(&rhs);
		if (!ptr)
			throw std::bad_cast{};
		return *ptr;
	}

	template <Copyable ValueType>
	ValueType any_cast(any&& rhs)
	{
		auto ptr = any_cast<ValueType>(&rhs);
		if (!ptr)
			throw std::bad_cast{};
		return std::move(*ptr);
	}

	template <Copyable ValueType>
	const ValueType* any_cast(const any* rhs) noexcept
	{
		if (!rhs || !rhs->instance_)
			return nullptr;
		auto* storage =  dynamic_cast<storage_impl<ValueType>*>(rhs->instance_.get());
		if (!storage)
			return nullptr;
		return &storage->value_type_;
	}

	template <Copyable ValueType>
	ValueType* any_cast(any* rhs) noexcept
	{
		return const_cast<ValueType*>(any_cast<ValueType>(static_cast<const any*>(rhs)));
	}
}
