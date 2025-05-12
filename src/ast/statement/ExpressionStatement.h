#pragma once
#include "Statement.h"
#include "../expression/Expression.h"

struct ExpressionStatement : public Statement
{
	Expression expression;
};