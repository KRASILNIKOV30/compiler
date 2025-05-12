#pragma once
#include "Expression.h"

struct NewExpression : public Expression
{
	std::string callee;
};