#pragma once
#include "../Entity.h"
#include <string>
#include <vector>

class Declaration : public Entity
{
public:
	Declaration(std::string id, Type type)
		: m_id(std::move(id))
		, m_type(std::move(type))
	{
	}

protected:
	[[nodiscard]] std::string GetId() const
	{
		return m_id;
	}

	[[nodiscard]] Type GetType() const
	{
		return m_type;
	};

private:
	std::string m_id;
	Type m_type;
};

using DeclarationPtr = std::unique_ptr<Declaration>;
