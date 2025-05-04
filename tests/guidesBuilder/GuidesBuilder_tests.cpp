#define CATCH_CONFIG_MAIN
#include "../../src/guidesBuilder/GuidesBuilder.h"
#include "catch.hpp"
#include "../../src/parseRawRules/StringifyRules.h"

void Check(std::string const& rawRules, std::string const& expected)
{
	GuidesBuilder builder(rawRules);
	const auto rules = builder.BuildGuidedRules();
	const auto rulesStr = StringifyRules(rules);
	CHECK(rulesStr == expected);
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
		"<S> - a # / a\n"
		"<A> - c <A> <S> / c\n"
		"<A> - c <S> / c\n");

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
		"<Z> - <E> # / <T> id <F> - (\n"
		"<E> - <T> <G> / <F> - id (\n"
		"<E> - <T> / <F> - id (\n"
		"<G> - + <T> <G> / +\n"
		"<G> - + <T> / +\n"
		"<T> - <F> <U> / ( - id\n"
		"<T> - <F> / ( - id\n"
		"<U> - * <F> <F> / *\n"
		"<F> - ( <E> ) / (\n"
		"<F> - - <F> / -\n"
		"<F> - id / id\n");

	Check(""
		"<Z> - <U> #\n"
		"<U> - <A> <B> <C>\n"
		"<A> - a <A> | e\n"
		"<B> - b <B> | e\n"
		"<C> - c <C> | e\n",
		""
		"<Z> - <U> # / <A> <C> <B> b c a\n"
		"<Z> - # / #\n"
		"<U> - <A> <B> <C> / a\n"
		"<U> - <B> <C> / b\n"
		"<U> - <A> <C> / a\n"
		"<U> - <C> / c\n"
		"<U> - <A> <B> / a\n"
		"<U> - <B> / b\n"
		"<U> - <A> / a\n"
		"<A> - a <A> / a\n"
		"<A> - a / a\n"
		"<B> - b <B> / b\n"
		"<B> - b / b\n"
		"<C> - c <C> / c\n"
		"<C> - c / c\n");

	Check(""
		"<S> - int <idList> #\n"
		"<idList> - <idList> , <id>\n"
		"<idList> - <id>\n"
		"<id> - A\n"
		"<id> - B\n"
		"<id> - C\n",
		""
		"<S> - int <idList> # / int\n"
		"<idList> - <idList> , <id> / <idList> <id> B C A\n"
		"<idList> - <id> / A C B\n"
		"<id> - A / A\n"
		"<id> - B / B\n"
		"<id> - C / C\n");

	Check(""
		"<S> - ( <optList> ) #\n"
		"<optList> - <list> | e\n"
		"<list> - <list> , id\n"
		"<list> - id\n",
		""
		"<S> - ( <optList> ) # / (\n"
		"<S> - ( ) # / (\n"
		"<optList> - <list> / <list> id\n"
		"<list> - <list> , id / <list> id\n"
		"<list> - id / id\n");
}
