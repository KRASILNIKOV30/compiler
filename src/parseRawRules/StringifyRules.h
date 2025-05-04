#pragma once
#include "ParseRules.h"
#include <string>
#include "../tableBuilder/parseRules/ParseRules.h"

std::string StringifyRules(raw::Rules const& rules);
std::string StringifyRules(Rules const& rules);