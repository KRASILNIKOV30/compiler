#pragma once
#include "Statement.h"
#include "../expression/Expression.h"
#include "BlockStatement.h"

struct WhileStatement : Statement
{
	void Generate(CodeGenerator& generator) const override
	{
		++whileId;

		generator.AddLabel("while" + whileId);
		condition.Generate(generator);
		generator.AddInstruction("jmpfalse endwhile" + whileId);

		body.Generate(generator);
		generator.AddInstruction("jmp while" + whileId);

		generator.AddLabel("endwhile" + whileId);
	}

	Expression condition;
	BlockStatement body;
	static int whileId = 0;
};