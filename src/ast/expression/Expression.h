#pragma once
#include "../Entity.h"
#include "../Type.h"
#include <memory>
#include <string>
#include <utility>

class Expression : public Entity
{
public:
	explicit Expression(Type type)
		: m_type(std::move(type))
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		throw std::runtime_error("Generating abstract expression");
	};

	[[nodiscard]] Type GetType() const
	{
		return m_type;
	}

	[[nodiscard]] virtual bool HasValue() const
	{
		return false;
	}

	[[nodiscard]] virtual std::string GetValue() const
	{
		return "";
	};

protected:
	Type m_type;
};

using ExpressionPtr = std::unique_ptr<Expression>;