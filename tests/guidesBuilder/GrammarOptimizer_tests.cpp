#include "catch.hpp"
#include "../../src/grammarOptimizer/GrammarOptimizer.h"
#include <sstream>

namespace
{

void Check(std::string const& input, std::string const& expected)
{
	std::stringstream inputStrm(input);
	std::stringstream expectedStrm(expected);
	const auto rules = ParseRawRules(inputStrm);
	GrammarOptimizer optimizer(rules);
	optimizer.RemoveLeftRecursion();
	const auto optimized = optimizer.GetRules();

	raw::Rules expectedRules = ParseRawRules(expectedStrm);

	REQUIRE(optimized.size() == expectedRules.size());
	for (size_t i = 0; i < optimized.size(); ++i)
	{
		REQUIRE(optimized[i].first == expectedRules[i].first);
		REQUIRE(optimized[i].second == expectedRules[i].second);
	}
}

}

TEST_CASE("Remove left recursion tests")
{
	SECTION("Simple immediate left recursion")
	{
		Check(""
			"<A> - <A> a\n"
			"<A> - b\n",
			""
			"<A> - b <ARem>\n"
			"<ARem> - a <ARem>\n"
			"<ARem> - e\n"
			);
	}
	SECTION("Multiple recursive alternatives")
	{
		Check(""
			"<B> - <B> c\n"
			"<B> - <B> d\n"
			"<B> - a\n"
			"<B> - f\n",
			""
			"<B> - a <BRem>\n"
			"<B> - f <BRem>\n"
			"<BRem> - c <BRem>\n"
			"<BRem> - d <BRem>\n"
			"<BRem> - e\n"
			);
	}

	SECTION("Mixed recursive and non-recursive")
	{
		Check(""
			"<C> - <C> x y\n"
			"<C> - z\n"
			"<C> - w <C>\n",
			""
			"<C> - z <CRem>\n"
			"<C> - w <C> <CRem>\n"
			"<CRem> - x y <CRem>\n"
			"<CRem> - e\n"
			);
	}

	SECTION("No left recursion")
	{
		Check(""
			"<D> - a b\n"
			"<D> - c d\n",
			""
			"<D> - a b\n"
			"<D> - c d\n"
			);
	}

	SECTION("Multiple nonterminals with recursion")
	{
		Check(""
			"<E> - <E> + <T>\n"
			"<E> - <T>\n"
			"<T> - <T> * <F>\n"
			"<T> - <F>\n"
			"<F> - ( <E> )\n"
			"<F> - id\n",
			""
			"<E> - <T> <ERem>\n"
			"<T> - <F> <TRem>\n"
			"<F> - ( <E> )\n"
			"<F> - id\n"
			"<ERem> - + <T> <ERem>\n"
			"<ERem> - e\n"
			"<TRem> - * <F> <TRem>\n"
			"<TRem> - e\n"
			);
	}
}