#include "catch.hpp"
#include "../../src/LLParser/LLParser.h"
#include "../../src/tableBuilder/TableBuilder.h"

TEST_CASE("ll-parser tests")
{
	TableBuilder builder(
		"<Z> - <number> <func> # / id int\n"
		"<number> - e / id\n"
		"<number> - int / int\n"
		"<func> - id ( <idList> ) / id\n"
		"<idList> - id <idListRem> / id\n"
		"<idListRem> - e / )\n"
		"<idListRem> - , id <idListRem> / ,\n");
	const auto table = builder.BuildTable();
	LLParser parser(table);

	CHECK(parser.Parse("func(id1, id2)"));
	CHECK(parser.Parse("object.create(id)"));
	CHECK_FALSE(parser.Parse("object.create(id"));
	CHECK(parser.GetError() == ErrorReason{
		.expected = { ")", "," },
		.received = Token{TokenType::ERROR, "", 16, Error::EMPTY_INPUT}});
	CHECK(parser.Parse("2 func(id1, id2)"));
	CHECK_FALSE(parser.Parse("2.2 func(id1, id2)"));
	CHECK(parser.GetError() == ErrorReason{
		.expected = { "id", "int" },
		.received = Token{TokenType::FLOAT, "2.2", 0, Error::NONE}});
	CHECK_FALSE(parser.Parse("2 id func(id1, id2)"));
	CHECK(parser.GetError() == ErrorReason{
		.expected = { "(" },
		.received = Token{TokenType::ID, "func", 5, Error::NONE}});
}
