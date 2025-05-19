#pragma once
#include "../CodeGenerator.h"
#include "ExpressionType.h"

struct Expression : public CodeGenerator
{
	ExpressionType type;

	void Generate() override
	{}
};