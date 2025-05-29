#pragma once
#include "../expression/Expression.h"
#include "Declaration.h"

struct VariableDeclaration : Declaration
{
	void Generate(CodeGenerator& generator) const override
	{
		const auto pos = generator.GetVariablePosOrAdd(id);
		generator.AddInstruction("const " + std::to_string(pos));
		init.Generate(generator);
		generator.AddInstruction("setlocal " + std::to_string(pos));
	}

	bool isConst = false;
	Expression init;
};