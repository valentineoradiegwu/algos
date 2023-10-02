#pragma once
namespace val::utils
{
	template <typename FUNCTOR>
	class scoped_action
	{
	public:
		scoped_action(FUNCTOR func)
			:func_{std::move(func)}
		{}
		~scoped_action() noexcept
		{
			func_();
		}
	private:
		FUNCTOR func_;
	};

	template <typename FUNCTOR>
	scoped_action<FUNCTOR> make_scoped_action(FUNCTOR func)
	{
		return scoped_action<FUNCTOR>{std::move(func)};
	}
}
