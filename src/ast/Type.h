#pragma once
#include "../parser/RemapToken.h"
#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

enum class PrimitiveType
{
	INT,
	FLOAT,
	STRING,
	BOOL,
	VOID,
	ANY,
};

struct Type;
using FunctionType = std::vector<Type>;
struct ArrayType;
using ArrayTypePtr = std::shared_ptr<ArrayType>;

struct Type
{
	std::variant<PrimitiveType, FunctionType, ArrayTypePtr> type;

	Type(PrimitiveType pt)
		: type(pt)
	{
	}

	Type(FunctionType ft)
		: type(ft)
	{
	}

	Type(ArrayTypePtr at)
		: type(std::move(at))
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

struct ArrayType
{
	ArrayType(Type type)
		: elementType(std::move(type))
	{
	}

	Type elementType;
};

inline bool operator==(ArrayTypePtr const& lhs, ArrayTypePtr const& rhs)
{
	return lhs->elementType == rhs->elementType;
}

inline std::unordered_map<TokenType, PrimitiveType> TokenTypeToPrimitiveType = {
	{ TokenType::INTEGER, PrimitiveType::INT },
	{ TokenType::FLOAT, PrimitiveType::FLOAT },
	{ TokenType::STRING_LITERAL, PrimitiveType::STRING },
	{ TokenType::TRUE, PrimitiveType::BOOL },
	{ TokenType::FALSE, PrimitiveType::BOOL },
	{ TokenType::TYPE_INTEGER, PrimitiveType::INT },
	{ TokenType::TYPE_FLOAT, PrimitiveType::FLOAT },
	{ TokenType::TYPE_BOOL, PrimitiveType::BOOL },
	{ TokenType::TYPE_STRING, PrimitiveType::STRING }
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
	case PrimitiveType::VOID:
		return "void";
	}

	throw std::runtime_error("Unknown primitive type");
}