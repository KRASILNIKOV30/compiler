#pragma once
#include <sstream>

class Reader
{
public:
	Reader(std::string const& str)
		: m_strm(str)
	{
	}

	std::string Get()
	{
		if (Empty())
		{
			return {};
		}

		++m_count;
		std::string str;
		m_strm >> str;
		return str;
	}

	char Peek()
	{
		SkipSpaces();
		return static_cast<char>(m_strm.peek());
	}

	std::string PeekN(const long n)
	{
		SkipSpaces();
		std::string str;
		for (size_t i = 0; i < n; ++i)
		{
			str += static_cast<char>(m_strm.get());
		}
		for (size_t i = 0; i < n; ++i)
		{
			m_strm.unget();
		}
		return str;
	}

	bool Empty()
	{
		Peek();
		return m_strm.eof();
	}

	size_t GetCount() const
	{
		return m_count;
	}

private:
	void SkipSpaces()
	{
		while (m_strm.peek() == ' ')
		{
			m_strm.get();
		}
	}

private:
	std::istringstream m_strm;
	size_t m_count = 0;
};