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

		generator.AddLabel("while" + std::to_string(whileId));
		condition.Generate(generator);
		generator.AddInstruction("jmp_false endwhile" + std::to_string(whileId));

		body.Generate(generator);
		generator.AddInstruction("jmp while" + std::to_string(whileId));

		generator.AddLabel("endwhile" + std::to_string(whileId));
	}

	Expression condition;
	BlockStatement body;
};