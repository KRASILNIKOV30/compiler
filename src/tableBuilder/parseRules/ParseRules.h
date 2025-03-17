#pragma once
#include <string>
#include <vector>
#include "../../table/Table.h"

const std::string EMPTY = "e";

using Alternative = std::pair<std::vector<std::string>, Guides>;
using Alternatives = std::vector<Alternative>;
using Rules = std::vector<std::pair<std::string, Alternatives>>;

Rules ParseRules(std::stringstream& input);
