#pragma once
#include "../Entity.h"
#include "../Type.h"
#include <memory>
#include <string>

class Expression : public Entity
{
public:
	explicit Expression(PrimitiveType type)
		: m_type(type)
	{
	}

	void Generate(CodeGenerator& generator) const override
	{
		throw std::runtime_error("Generating abstract expression");
	};

	[[nodiscard]] PrimitiveType GetType() const
	{
		return m_type;
	}

protected:
	PrimitiveType m_type;
};

using ExpressionPtr = std::unique_ptr<Expression>;