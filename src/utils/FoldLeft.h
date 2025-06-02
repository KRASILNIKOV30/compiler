#pragma once
#include <functional>
#include <ranges>

namespace
{

struct FoldLeftFn
{
	template <std::input_iterator I, std::sentinel_for<I> S, class T = std::iter_value_t<I>, class F>
	constexpr auto operator()(I first, S last, T init, F f) const
	{
		using U = std::decay_t<std::invoke_result_t<F&, T, std::iter_reference_t<I>>>;
		if (first == last)
		{
			return U(std::move(init));
		}

		U accum = std::invoke(f, std::move(init), *first);

		for (++first; first != last; ++first)
		{
			accum = std::invoke(f, std::move(accum), *first);
		}

		return std::move(accum);
	}

	template <std::ranges::input_range R, class T = std::ranges::range_value_t<R>, class F>
	constexpr auto operator()(R&& r, F f, T init = T()) const
	{
		return (*this)(std::ranges::begin(r), std::ranges::end(r), std::move(init), std::ref(f));
	}
};

} // namespace

inline constexpr FoldLeftFn FoldLeft;