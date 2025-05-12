#pragma once
#include "Declaration.h"
#include "../expression/Expression.h"

struct VariableDeclaration : public Declaration
{
	bool isConst;
	Expression init;
};