#include <catch.hpp>
#include "../src/lexer/Lexer.h"

template <typename... Args>
void Check(Lexer lexer, Args&&... args)
{
	auto checkOne = [&lexer](auto token) {
		const auto [type, value, pos, error] = lexer.Get();
		CHECK(type == token.type);
		CHECK(value == token.value);
		CHECK(pos == token.pos);
		CHECK(error == token.error);
	};
	(checkOne(args), ...);
}

TEST_CASE("empty lexer")
{
	Check(Lexer(""),
		Token{ TokenType::ERROR, "", 0, Error::EMPTY_INPUT }
		);

	Check(Lexer("    "),
		Token{ TokenType::ERROR, "", 0, Error::EMPTY_INPUT }
		);
}

TEST_CASE("valid id tests")
{

	Check(Lexer("id secondId    m_id a _$s123 plus+"),
		Token{ TokenType::ID, "id", 0 },
		Token{ TokenType::ID, "secondId", 3 },
		Token{ TokenType::ID, "m_id", 15 },
		Token{ TokenType::ID, "a", 20 },
		Token{ TokenType::ID, "_$s123", 22 },
		Token{ TokenType::ID, "plus", 29 }
		);
}

TEST_CASE("invalid id tests")
{
	Check(Lexer("obj.id.field error."),
		Token{ TokenType::ID, "obj.id.field", 0 },
		Token{ TokenType::ERROR, "", 13, Error::INVALID_ID }
		);
}


TEST_CASE("valid numbers tests")
{
	Check(Lexer("5 0 123 12.52 0.25 0.0013 1e+10 100.53E-15"),
		Token{ TokenType::INTEGER, "5", 0 },
		Token{ TokenType::INTEGER, "0", 2 },
		Token{ TokenType::INTEGER, "123", 4 },
		Token{ TokenType::FLOAT, "12.52", 8 },
		Token{ TokenType::FLOAT, "0.25", 14 },
		Token{ TokenType::FLOAT, "0.0013", 19 },
		Token{ TokenType::FLOAT, "1e+10", 26 },
		Token{ TokenType::FLOAT, "100.53E-15", 32 }
		);
}

TEST_CASE("invalid numbers tests")
{
	Check(Lexer("05"), Token{ TokenType::ERROR, "", 0, Error::INVALID_NUMBER });
	Check(Lexer("00.5"), Token{ TokenType::ERROR, "", 0, Error::INVALID_NUMBER });
	Check(Lexer("12e+"), Token{ TokenType::ERROR, "", 0, Error::INVALID_NUMBER });
	Check(Lexer("12.e+10"), Token{ TokenType::ERROR, "", 0, Error::INVALID_NUMBER });
	Check(Lexer("42.53e"), Token{ TokenType::ERROR, "", 0, Error::INVALID_NUMBER });
	Check(Lexer(" 42.53e10"), Token{ TokenType::ERROR, "", 1, Error::INVALID_NUMBER });
	Check(Lexer("  42.53e+0"), Token{ TokenType::ERROR, "", 2, Error::INVALID_NUMBER });
}

TEST_CASE("valid string tests")
{
	Check(Lexer("'' 'Hello, World!'"),
		Token{ TokenType::STRING_LITERAL, "''", 0 },
		Token{ TokenType::STRING_LITERAL, "'Hello, World!'", 3 }
		);
}

TEST_CASE("invalid string tests")
{
	Check(Lexer("'"), Token{ TokenType::ERROR, "", 0, Error::STRING_LITERAL_INCOMPLETE });
	Check(Lexer("   'Hello"), Token{ TokenType::ERROR, "", 3, Error::STRING_LITERAL_INCOMPLETE });
}

TEST_CASE("reserved words tests")
{
	Check(Lexer("mod div and or not true false"),
		Token{ TokenType::OP_MOD, "mod", 0 },
		Token{ TokenType::OP_DIV, "div", 4 },
		Token{ TokenType::OP_AND, "and", 8 },
		Token{ TokenType::OP_OR, "or", 12 },
		Token{ TokenType::OP_NOT, "not", 15 },
		Token{ TokenType::TRUE, "true", 19 },
		Token{ TokenType::FALSE, "false", 24 }
		);

	Check(Lexer("nott"), Token{ TokenType::ID, "nott", 0 });
}

