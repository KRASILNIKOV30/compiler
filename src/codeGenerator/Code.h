#pragma once
#include <string>
#include <sstream>

class Code
{
public:
	Code() = default;

	Code& operator+=(std::string const& str)
	{
		m_code += str;
		return *this;
	}

	Code operator+(std::string const& str) const
	{
		Code result = *this;
		result += str;
		return result;
	}

	template<typename T>
	Code& operator<<(T const& value)
	{
		m_code += value;
		return *this;
	}

	Code& operator<<(std::ostream& (*manip)(std::ostream&)) {
		std::ostringstream oss;
		manip(oss);
		m_code += oss.str();
		return *this;
	}

	const std::string& Get() const
	{
		return m_code;
	}

private:
	std::string m_code;
};