#pragma once
#include "Declaration.h"
#include "../expression/Expression.h"

struct VariableDeclaration : Declaration
{
	void Generate(CodeGenerator& generator) const override
	{
		 generator.AddInstruction("const " + generator.GetVariablePosOrAdd(id));
	}

	bool isConst = false;
	Expression init;
};