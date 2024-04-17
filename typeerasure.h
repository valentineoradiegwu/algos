#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <typeinfo>

struct Concept
{
	virtual std::string get_underlying_type() const noexcept = 0;
	virtual std::unique_ptr<Concept> clone() const = 0;
	virtual ~Concept() {};
};

template <typename T>
struct Model : Concept
{
	template <typename... ARGS>
	Model(ARGS&&... args)
		:val_{ std::forward<ARGS>(args)... }
	{}
	std::string get_underlying_type() const noexcept override
	{
		const std::type_info& r1 = typeid(val_);
		return r1.name();
	}
	std::unique_ptr<Concept> clone() const override
	{
		return std::make_unique<Model<T>>(val_);
	}
	T val_;
};

class Insertable
{
public:
	template <typename T>
	Insertable(T&& val)
		:handle_{ std::make_unique<Model<T>>(std::forward<T>(val)) }
	{};
	Insertable(const Insertable& rhs) noexcept
		:handle_{ rhs.handle_ ? rhs.handle_->clone() : nullptr }
	{};
	Insertable(Insertable&& rhs) noexcept
		:handle_(std::move(rhs.handle_))
	{};
	Insertable& operator=(const Insertable& rhs) noexcept
	{
		handle_ = rhs.handle_ ? rhs.handle_->clone() : nullptr;
		return *this;
	}
	Insertable& operator=(Insertable&& rhs) noexcept
	{
		handle_ = std::move(rhs.handle_);
		return *this;
	}

	std::string get_underlying_type() const noexcept
	{
		return handle_->get_underlying_type();
	}
private:
	std::unique_ptr<Concept> handle_;
};
