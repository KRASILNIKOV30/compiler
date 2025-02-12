#pragma once
#include "./token/Token.h"
#include "reader/Reader.h"
#include <catch.hpp>

class Lexer
{
public:
	explicit Lexer(std::string const& input);
	Token Get();
	[[nodiscard]] bool Empty();

private:
	Token Id();
	void SkipWhitespaces();

private:
	Reader m_reader;
};

