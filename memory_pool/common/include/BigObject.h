#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>

class BigObject
{
public:
    explicit BigObject(int id = 0) noexcept
        : m_id(id)
    {
    }

    ~BigObject() = default;

    [[nodiscard]]
    int id() const noexcept
    {
        return m_id;
    }

    void setId(int id) noexcept
    {
        m_id = id;
    }

    void reset() noexcept
    {
        std::fill(std::begin(m_data), std::end(m_data), 0);
        m_id = 0;
    }

private:
    int m_data[1000]{};
    int m_id{};
};



