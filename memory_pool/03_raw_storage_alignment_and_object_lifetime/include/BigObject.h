#pragma once

#include <iostream>

class BigObject
{
public:
	explicit BigObject(int id = 0) : m_id(id)
	{
		std::cout << "BigObject constructed: " << m_id << '\n';
	}
	
	~BigObject()
	{
		std::cout << "BigObject destroyed: " << m_id << '\n';
	}
	
	[[nodiscard]] int id() const noexcept
	{
		return m_id;
	}

private:
	int m_data[1000]{};
	int m_id{};
};
