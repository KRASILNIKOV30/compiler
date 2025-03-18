#include "catch.hpp"
#include "../../src/guidesBuilder/GuidesBuilder.h"

void Check(std::string const& rawRules, std::string const& expected)
{
	GuidesBuilder builder(rawRules);
	const auto rules = builder.BuildGuidedRules();
	CHECK(rules.has_value());
	CHECK(rules.value() == expected);
}

TEST_CASE("guides builder tests")
{
	Check(""
		"<S> - a <A> #\n"
		"<S> - b\n"
		"<A> - c <A> <S>\n"
		"<A> - e\n",
		""
		"<S> - a <A> # / a\n"
		"<S> - b / b\n"
		"<A> - c <A> <S> / c\n"
		"<A> - e / # b a\n");

	Check(""
		"<Z> - <E> #\n"
		"<E> - <T> <G>\n"
		"<G> - + <T> <G>\n"
		"<G> - e\n"
		"<T> - <F> <U>\n"
		"<U> - * <F> <F>\n"
		"<U> - e\n"
		"<F> - ( <E> )\n"
		"<F> - - <F>\n"
		"<F> - id\n",
		""
		"<Z> - <E> # / ( - id\n"
		"<E> - <T> <G> / ( id -\n"
		"<G> - + <T> <G> / +\n"
		"<G> - e / # )\n"
		"<T> - <F> <U> / id - (\n"
		"<U> - * <F> <F> / *\n"
		"<U> - e / + # )\n"
		"<F> - ( <E> ) / (\n"
		"<F> - - <F> / -\n"
		"<F> - id / id\n");

	Check(""
		"<Z> - <U> #\n"
		"<U> - <A> <B> <C>\n"
		"<A> - a <A>\n"
		"<A> - e\n"
		"<B> - b <B>\n"
		"<B> - e\n"
		"<C> - c <C>\n"
		"<C> - e\n",
		""
		"<Z> - <U> # / # a b c\n"
		"<U> - <A> <B> <C> / # c b a\n"
		"<A> - a <A> / a\n"
		"<A> - e / b c #\n"
		"<B> - b <B> / b\n"
		"<B> - e / c #\n"
		"<C> - c <C> / c\n"
		"<C> - e / #\n");
}
