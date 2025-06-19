#pragma once
#include "../ast/expression/Expression.h"

enum class AdapterType
{
	DROP,
	TAKE,
	REVERSE,
	FILTER,
	TRANSFORM,
};

struct Adapter
{
	AdapterType type;
	ExpressionPtr expr = nullptr;
};

inline AdapterType GetAdapterType(TokenType const& type)
{
	switch (type)
	{
	case TokenType::DROP:
		return AdapterType::DROP;
	case TokenType::TAKE:
		return AdapterType::TAKE;
	case TokenType::REVERSE:
		return AdapterType::REVERSE;
	case TokenType::FILTER:
		return AdapterType::FILTER;
	case TokenType::TRANSFORM:
		return AdapterType::TRANSFORM;
	}

	throw std::logic_error("Can not convert " + RemapTokenTypeToString(type) + " to adapter.");
}