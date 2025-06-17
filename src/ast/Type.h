#pragma once
#include "../parser/RemapToken.h"
#include <stdexcept>
#include <variant>
#include <vector>

enum class PrimitiveType
{
	INT,
	FLOAT,
	STRING,
	BOOL,
	VOID,
};

struct Type;
using FunctionType = std::vector<Type>;
struct Type
{
	std::variant<PrimitiveType, FunctionType> type;

	Type(PrimitiveType pt)
		: type(pt)
	{
	}

	Type(FunctionType ft)
		: type(ft)
	{
	}

	Type(std::initializer_list<PrimitiveType> list)
	{
		FunctionType funcTypeVec;
		funcTypeVec.reserve(list.size());

		for (auto pt : list)
		{
			funcTypeVec.emplace_back(pt);
		}

		type = std::move(funcTypeVec);
	}

	bool operator==(const Type& rhs) const = default;
};

inline std::unordered_map<TokenType, PrimitiveType> TokenTypeToPrimitiveType = {
	{ TokenType::INTEGER, PrimitiveType::INT },
	{ TokenType::FLOAT, PrimitiveType::FLOAT },
	{ TokenType::STRING_LITERAL, PrimitiveType::STRING },
	{ TokenType::TRUE, PrimitiveType::BOOL },
	{ TokenType::FALSE, PrimitiveType::BOOL }
};

inline PrimitiveType GetPrimitiveType(TokenType const& tokenType)
{
	const auto& it = TokenTypeToPrimitiveType.find(tokenType);
	if (it != TokenTypeToPrimitiveType.end())
	{
		return it->second;
	}
	throw std::runtime_error("Cannot convert " + RemapTokenTypeToString(tokenType) + " to primitive type");
}

inline std::string StringifyPrimitiveType(PrimitiveType const& type)
{
	switch (type)
	{
	case PrimitiveType::INT:
	case PrimitiveType::FLOAT:
		return "number";
	case PrimitiveType::STRING:
		return "string";
	case PrimitiveType::BOOL:
		return "bool";
	}

	throw std::runtime_error("Unknown primitive type");
}