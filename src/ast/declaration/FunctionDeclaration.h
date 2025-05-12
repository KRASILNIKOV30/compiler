#pragma once
#include "Declaration.h"
#include "../statement/BlockStatement.h"

struct FunctionDeclaration : public Declaration
{
	BlockStatement body;
};