#pragma once
#include <iostream>
#include <sstream>

/**
 * Производит посимвольное чтение и подсчет количества прочитанных символов
 */
class Reader
{
public:
	explicit Reader(const std::string& str)
		: m_input(str)
	{
	}

	explicit Reader(const std::istream& strm)
	{
		m_input << strm.rdbuf();
	}

	char Get()
	{
		if (EndOfFile())
		{
			throw std::runtime_error("EOF Error: tried to get char from an empty reader");
		}
		const auto ch = static_cast<char>(m_input.get());
		if (ch == '\n')
		{
			++m_lineCount;
			m_count = 0;
		}
		else
		{
			++m_count;
		}
		m_record += ch;

		return ch;
	}

	[[nodiscard]] char Peek()
	{
		return static_cast<char>(m_input.peek());
	}

	void Unget()
	{
		--m_count;
		m_record.pop_back();
		m_input.unget();
	}

	[[nodiscard]] size_t Count() const
	{
		return m_count;
	}

	[[nodiscard]] size_t LineCount() const
	{
		return m_lineCount;
	}

	[[nodiscard]] bool Empty()
	{
		return m_input.peek() == '\n' || m_input.eof();
	}

	[[nodiscard]] bool EndOfFile()
	{
		m_input.peek();
		return m_input.eof();
	}

	void Seek(const size_t pos)
	{
		const size_t dropLen = m_count - pos;
		if (dropLen <= m_record.size())
		{
			m_record.resize(m_record.size() - dropLen);
		}
		else
		{
			m_record.clear();
		}
		const auto offset = pos - m_count;
		m_count = pos;
		m_input.clear();
		m_input.seekg(static_cast<long>(offset), std::ios_base::cur);
	}

	void Record()
	{
		m_record.clear();
	}

	std::string StopRecord()
	{
		return m_record;
	}

private:
	std::stringstream m_input;
	size_t m_count = 0;
	size_t m_lineCount = 0;
	std::string m_record;
};