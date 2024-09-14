#pragma once
#include <memory>

//https://stackoverflow.com/questions/75962554/why-is-stdfunction-implemented-as-a-partial-template-specialisation-with-an-em

namespace val::utils
{
	template <typename>
	class function;

	template<typename RET, typename... PARAMS>
	class function<RET(PARAMS...)>
	{
	public:
		function() = default;

		function(const function& rhs)
			: func{ rhs.func ? rhs.func->clone() : nullptr}
		{}

		function(function&& rhs)
			:func{std::move(rhs.func)}
		{}

		function& operator=(function rhs)
		{
			std::swap(rhs.func, func);
			return *this;
		}

		function& operator=(function&& rhs)
		{
			auto cpy = function{std::move(rhs)};
			std::swap(cpy.func, func);
			return *this;
		}

		template <typename FUN>
		function(FUN&& fun)
			: func{ std::make_unique<model<FUN>>(std::forward<FUN>(fun)) }
		{}
	private:
		struct callable
		{
			virtual std::unique_ptr<callable> clone() const = 0;
			virtual RET invoke(PARAMS&&... ARGS) = 0;
			virtual ~callable() = default;
		};

		template <template T>
		struct model : callable
		{
			model(T&& functor)
				: invokable(std::forward<T>(functor))
			{}

			std::unique_ptr<callable> clone() const override
			{
				return std::make_unique<model<T>>(invokable);
			}

			RET invoke(PARAMS&&... ARGS)
			{
				return invokable(std::forward<PARAMS>(ARGS)...);
			}
			T invokable;
		};
		std::unique_ptr<callable> func;
	};
}
