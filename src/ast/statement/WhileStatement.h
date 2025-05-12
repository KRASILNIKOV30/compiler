#pragma once
#include "Statement.h"
#include "../expression/Expression.h"
#include "BlockStatement.h"

struct WhileStatement : public Statement
{
	Expression condition;
	BlockStatement body;
};