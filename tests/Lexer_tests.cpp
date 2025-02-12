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

TEST_CASE("id tests")
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


