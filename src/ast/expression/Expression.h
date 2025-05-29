#pragma once
#include "ExpressionType.h"
#include "../Entity.h"
#include <string>

struct Expression : Entity
{
	void Generate(CodeGenerator& generator) const override
	{
		throw std::runtime_error("Generating abstract expression");
	};

	ExpressionType type;
};