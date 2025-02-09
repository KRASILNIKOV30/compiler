#pragma once
#include <iostream>

/**
 * Производит посимвольное чтение и подсчет количества прочитанных символов
 */
class Reader
{
public:
	explicit Reader(std::istream& input)
		: m_input(input)
	{
	}

	char Get()
	{
		if (Empty())
		{
			throw std::runtime_error("EOF Error: tried to get char from an empty reader");
		}
		++m_count;
		return static_cast<char>(m_input.get());
	}

	[[nodiscard]] char Peek() const
	{
		return static_cast<char>(m_input.peek());
	}

	void Unget()
	{
		--m_count;
		m_input.unget();
	}

	[[nodiscard]] size_t Count() const
	{
		return m_count;
	}

	[[nodiscard]] bool Empty() const
	{
		m_input.peek();
		return m_input.eof();
	}

	void Seek(const size_t pos)
	{
		m_count = pos;
		m_input.seekg(static_cast<long>(pos));
	}

private:
	std::istream& m_input;
	size_t m_count = 0;
};