#pragma once
#include "../ast/Type.h"
#include <deque>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>

struct Symbol
{
	bool isConst;
	Type type;
};

class SymbolTable
{

public:
	SymbolTable()
	{
		m_scopes.emplace_back();
		DefNativeFunctions();
	}

	void CreateScope()
	{
		m_scopes.emplace_back();
	}

	void DeleteScope()
	{
		m_scopes.pop_back();
	}

	Symbol Get(std::string const& name)
	{
		const auto symbol = Find(name);
		if (!symbol.has_value())
		{
			throw std::runtime_error("Symbol " + name + " is undefined");
		}
		return symbol.value();
	}

	void Add(std::string const& name, Symbol const& symbol)
	{
		if (FindInCurrentScope(name).has_value())
		{
			throw std::runtime_error("Symbol " + name + " is already defined");
		}
		m_scopes.back().emplace(name, symbol);
	}

private:
	using Scope = std::unordered_map<std::string, Symbol>;

	void DefNativeFunctions()
	{
		Add("print", { true, FunctionType{ PrimitiveType::STRING, PrimitiveType::VOID } });
		Add("println", { true, FunctionType{ PrimitiveType::STRING, PrimitiveType::VOID } });
		Add("getTimestamp", { true, FunctionType{ PrimitiveType::VOID, PrimitiveType::INT } });
		Add("sqrt", { true, FunctionType{ PrimitiveType::INT, PrimitiveType::INT } });
	}

	std::optional<Symbol> Find(std::string const& name)
	{
		for (const auto& scope : m_scopes | std::views::reverse)
		{
			const auto symbol = FindInScope(name, scope);
			if (symbol.has_value())
			{
				return symbol;
			}
		}
		return std::nullopt;
	}

	std::optional<Symbol> FindInCurrentScope(std::string const& name)
	{
		return FindInScope(name, m_scopes.back());
	}

	static std::optional<Symbol> FindInScope(std::string const& name, Scope const& scope)
	{
		const auto it = scope.find(name);
		if (it != scope.end())
		{
			return it->second;
		}
		return std::nullopt;
	}

private:
	std::deque<Scope> m_scopes;
};