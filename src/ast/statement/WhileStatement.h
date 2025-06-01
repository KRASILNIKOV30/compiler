#pragma once
#include "Statement.h"
#include "../expression/Expression.h"
#include "BlockStatement.h"

struct WhileStatement : Statement
{
	void Generate(CodeGenerator& generator) const override
	{
		static int whileId = 0;
		++whileId;

		generator.AddLabel("while" + whileId);
		condition.Generate(generator);
		generator.AddInstruction("jmp_false endwhile" + whileId);

		body.Generate(generator);
		generator.AddInstruction("jmp while" + whileId);

		generator.AddLabel("endwhile" + whileId);
	}

	Expression condition;
	BlockStatement body;
};