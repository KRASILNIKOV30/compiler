#include <utility>

#include "../parseRawRules/ParseRules.h"

class GrammarOptimizer
{
public:
	explicit GrammarOptimizer(raw::Rules rules)
		: m_rules(std::move(rules))
	{
	}

	void RemoveLeftRecursion();
	void LeftFactor();
	[[nodiscard]] raw::Rules const& GetRules() const;

private:
	raw::Rules m_rules;
};
