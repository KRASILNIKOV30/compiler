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
}
