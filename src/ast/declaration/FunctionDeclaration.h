#pragma once
#include "../statement/BlockStatement.h"
#include "Declaration.h"

struct FunctionDeclaration : Declaration
{
	void Generate(CodeGenerator& generator) const override
	{
		static int functionId = 0;

		generator.AddInstruction(".function" + std::to_string(++functionId));
		body.Generate(generator);
		generator.AddInstruction("return");
	}

	BlockStatement body;
};