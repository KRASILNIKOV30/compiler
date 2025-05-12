#pragma once
#include "Statement.h"
#include "../expression/Expression.h"

struct ReturnStatement : public Statement
{
	Expression argument;
};