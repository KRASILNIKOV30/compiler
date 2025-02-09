#pragma once

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
		++m_pos;
		return static_cast<char>(m_input.get());
	}

	[[nodiscard]] char Peek() const
	{
		return static_cast<char>(m_input.peek());
	}

	void Unget()
	{
		--m_pos;
		m_input.unget();
	}

	[[nodiscard]] size_t GetPos() const
	{
		return m_pos;
	}

	[[nodiscard]] bool Empty() const
	{
		m_input.peek();
		return m_input.eof();
	}

private:
	std::istream& m_input;
	size_t m_pos = 0;
};