#pragma once
#include "Expression.h"
#include <vector>

struct Property
{
	std::string key;
	Expression value;
};

struct ObjectExpression : Expression
{
	std::vector<Property> properties;
};