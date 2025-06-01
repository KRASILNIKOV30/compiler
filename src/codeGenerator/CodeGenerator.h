#pragma once
#include "../ast/Type.h"
#include <format>
#include <sstream>
#include <string>
#include <vector>

class CodeGenerator
{
public:
	CodeGenerator() = default;

	void AddInstruction(std::string const& code, bool isNewRow = false)
	{
		m_rowId += isNewRow;
		m_codeBlock << m_rowId << " " << code << std::endl;
	}

	void AddComment(std::string const& comment)
	{
		m_codeBlock << "//" << comment << std::endl;
	}

	void AddLabel(std::string const& label)
	{
		m_codeBlock << label << ":" << std::endl;
	}

	void AddConstant(std::string const& type, std::string const& value)
	{
		m_codeBlock << type << " " << value << std::endl;
	}

	size_t GetVariablePos(std::string const& variableName)
	{
		const auto pos = std::ranges::find(m_variables, variableName) - m_variables.begin();
		if (pos >= m_variables.size())
		{
			throw std::invalid_argument("Variable" + variableName + " not found");
		}
		return pos;
	}

	size_t GetConstantPos(Type const& type, std::string const& value)
	{
		const auto pos = std::ranges::find(m_constants, std::make_pair(type, value)) - m_constants.begin();
		if (pos >= m_constants.size())
		{
			throw std::invalid_argument("Constant" + value + " not found");
		}
		return pos;
	}

	size_t GetVariablePosOrAdd(std::string const& variableName)
	{
		return GetValuePosOrAdd<std::string>(m_variables, variableName);
	}

	size_t GetConstantPosOrAdd(Type const& type, std::string const& value)
	{
		return GetValuePosOrAdd<std::pair<Type, std::string>>(m_constants, std::make_pair(type, value));
	}

	void PrintCode(std::ostream& outFile)
	{
		outFile << ".def" << std::endl
				<< ".argc 0" << std::endl
				<< ".locals " << m_variables.size() << std::endl
				<< ".name __EntryPoint__" << std::endl << std::endl;

		if (!m_constants.empty())
		{
			outFile << ".constants" << std::endl;
		}
		for (auto const& [type, value] : m_constants)
		{
			// FIXME: Hardcoded. Fix when adding arrow functions
			const auto outputType = type.back();
			const auto constValue = outputType == PrimitiveType::STRING ? std::format("\"{}\"", value) : value;
			outFile << StringifyPrimitiveType(outputType) << " " << constValue << std::endl;
		}

		outFile << std::endl
				<< ".code" << std::endl
				<< m_codeBlock.str()
				<< m_rowId << " return" << std::endl;
	}

private:
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

	std::ostringstream m_codeBlock;
	std::vector<std::string> m_variables;
	std::vector<std::pair<Type, std::string>> m_constants;
	int m_rowId = 1;
};