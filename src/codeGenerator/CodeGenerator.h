#pragma once
#include "../ast/Type.h"
#include "../utils/FoldLeft.h"
#include "Code.h"
#include "CodeLabelGenerator.h"
#include <format>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

class CodeGenerator
{
public:
	struct VariableContext
	{
		size_t pos;
		bool isVariableFromParent;
	};

	CodeGenerator()
	{
		m_functionNames.emplace_back(MAIN);
		m_functions.emplace(std::piecewise_construct, std::forward_as_tuple(MAIN), std::forward_as_tuple(MAIN, MAIN));
	}

	CodeLabelGenerator& GetLabelGenerator() { return m_labelGenerator; }

	void AddInstruction(std::string const& code, bool isNewRow = false)
	{
		m_rowId += isNewRow;
		GetCurrentContext().code << std::to_string(m_rowId) << " " << code << std::endl;
	}

	void AddLabel(std::string const& label)
	{
		GetCurrentContext().code << label << ":" << std::endl;
	}

	size_t GetVariablePosOrAdd(std::string const& variableName)
	{
		return GetValuePosOrAdd<std::string>(GetCurrentContext().variables, variableName);
	}

	size_t AddParentLocal(std::string const& variableName)
	{
		return GetValuePosOrAdd<Variable>(GetCurrentContext().parentLocals, std::make_pair(variableName, 0));
	}

	VariableContext GetVariableContextPos(std::string const& variableName)
	{
		auto& currentContext = GetCurrentContext();
		const auto result = FindVariableInContext(GetContext(currentContext.name), variableName);
		if (result.second == ParentContextVariableType::UNDEFINED)
		{
			throw std::invalid_argument("Undefined variable: " + variableName);
		}

		return { result.first, result.second == ParentContextVariableType::UPVALUE };
	}

	size_t GetConstantPosOrAdd(Type const& type, std::string const& value)
	{
		return GetValuePosOrAdd<std::pair<Type, std::string>>(GetCurrentContext().constants, std::make_pair(type, value));
	}

	void BeginFunction(std::string const& functionName, size_t argc)
	{
		m_functions.emplace(std::piecewise_construct, std::forward_as_tuple(functionName), std::forward_as_tuple(functionName, m_currentLocation, argc));
		m_functionNames.emplace_back(functionName);
		m_currentLocation = functionName;
	}

	void EndFunction()
	{
		m_currentLocation = GetCurrentContext().parentName;
	}

	void PrintCode(std::ostream& outFile)
	{
		for (auto const& functionName : m_functionNames)
		{
			const auto functionContext = m_functions.at(functionName);
			outFile << ".def" << std::endl
					<< ".argc " << functionContext.argc << std::endl
					<< ".locals " << functionContext.variables.size() - functionContext.argc << std::endl
					<< ".name " << functionContext.name << std::endl;

			if (!functionContext.constants.empty())
			{
				outFile << ".constants" << std::endl;
				for (auto const& [type, value] : functionContext.constants)
				{
					const auto outputType = get<PrimitiveType>(type.type);
					outFile << StringifyPrimitiveType(outputType) << " " << value << std::endl;
				}
			}
			if (!functionContext.parentLocals.empty())
			{
				outFile << ".parent_local"
						<< FoldLeft(
							   functionContext.parentLocals | std::views::transform([&](const auto& p) { return std::to_string(p.second); }),
							   [&](const auto& acc, const auto& curr) {
								   return acc + " " += curr;
							   })
						<< std::endl;
			}
			if (!functionContext.parentUpvalues.empty())
			{
				outFile << ".parent_upvalue"
						<< FoldLeft(
							   functionContext.parentUpvalues | std::views::transform([&](const auto& p) { return std::to_string(p.second); }),
							   [&](const auto& acc, const auto& curr) {
								   return acc + " " += curr;
							   })
						<< std::endl;
			}

			outFile << ".code" << std::endl
					<< functionContext.code.Get()
					<< std::to_string(m_rowId) << " return" << std::endl
					<< ".end_def" << std::endl
					<< std::endl;
		}
	}

private:
	using Constant = std::pair<Type, std::string>;
	using Variable = std::pair<std::string, size_t>;

	struct FunctionContext
	{
		std::string name;
		std::string parentName;
		size_t argc = 0;

		std::vector<std::string> variables;
		std::vector<Variable> parentLocals;
		std::vector<Variable> parentUpvalues;
		std::vector<Constant> constants;
		Code code;
	};

	enum class ParentContextVariableType
	{
		LOCAL,
		UPVALUE,
		UNDEFINED
	};

	FunctionContext& GetCurrentContext()
	{
		return m_functions.at(m_currentLocation);
	}

	FunctionContext& GetContext(std::string const& name)
	{
		return m_functions.at(name);
	}

	std::pair<size_t, ParentContextVariableType> FindVariableInContext(FunctionContext& context, std::string const& variableName)
	{
		size_t pos = std::ranges::find(context.variables, variableName) - context.variables.begin();
		if (pos < context.variables.size())
		{
			return { pos, ParentContextVariableType::LOCAL };
		}

		pos = std::ranges::find_if(context.parentLocals, [&](auto const& val) {
			return val.first == variableName;
		}) - context.parentLocals.begin();
		if (pos < context.parentLocals.size())
		{
			return { pos, ParentContextVariableType::UPVALUE };
		}

		pos = std::ranges::find_if(context.parentUpvalues, [&](auto const& val) {
			return val.first == variableName;
		}) - context.parentUpvalues.begin();
		if (pos < context.parentUpvalues.size())
		{
			return { context.parentLocals.size() + pos, ParentContextVariableType::UPVALUE };
		}

		if (context.name == MAIN)
		{
			return { -1, ParentContextVariableType::UNDEFINED };
		}

		auto result = FindVariableInContext(GetContext(context.parentName), variableName);
		if (result.second == ParentContextVariableType::UNDEFINED)
		{
			return { -1, ParentContextVariableType::UNDEFINED };
		}
		if (result.second == ParentContextVariableType::UPVALUE)
		{
			context.parentUpvalues.emplace_back(variableName, result.first);
			return { context.parentLocals.size() + context.parentUpvalues.size() - 1, ParentContextVariableType::UPVALUE };
		}

		context.parentLocals.emplace_back(variableName, result.first);
		return { context.parentLocals.size() - 1, ParentContextVariableType::UPVALUE };
	}

	template <typename T>
	static size_t GetValuePosOrAdd(std::vector<T>& arr, T const& value)
	{
		for (size_t i = 0; i < arr.size(); ++i)
		{
			if (arr[i] == value)
			{
				return i;
			}
		}
		arr.push_back(value);
		return arr.size() - 1;
	}

	int m_rowId = 1;
	std::string m_currentLocation = MAIN;
	std::vector<std::string> m_functionNames;
	std::unordered_map<std::string, std::string> m_functionToParent;
	std::unordered_map<std::string, FunctionContext> m_functions; // functionName -> context

	CodeLabelGenerator m_labelGenerator{};

	static inline const std::string MAIN = "__EntryPoint__";
};