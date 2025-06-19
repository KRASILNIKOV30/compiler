#pragma once
#include <string>

class CodeLabelGenerator
{
public:
	CodeLabelGenerator() = default;

	std::string GetElseLabel() const { return "else" + std::to_string(m_ifLabelId); }
	std::string GetEndIfLabel() const { return "endif" + std::to_string(m_ifLabelId); }
	void NewIfLabel() { ++m_ifLabelId; };

	std::string GetWhileLabel() const { return "while" + std::to_string(m_whileLabelId); }
	std::string GetEndWhileLabel() { return "endwhile" + std::to_string(m_whileLabelId); }
	void NewWhileLabel() { ++m_whileLabelId; };

	std::string GetArrowFunctionLabel() const { return "arrowFunction" + std::to_string(m_arrowFunctionLabelId); }
	int GetArrowFunctionLabelId() const { return m_arrowFunctionLabelId; }
	void NewArrowFunctionLabel() { ++m_arrowFunctionLabelId; };

private:
	int m_ifLabelId = 0;
	int m_whileLabelId = 0;
	int m_arrowFunctionLabelId = 0;
};