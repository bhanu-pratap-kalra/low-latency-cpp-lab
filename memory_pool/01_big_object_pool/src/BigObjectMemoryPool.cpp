#include "BigObjectMemoryPool.h"

BigObjectMemoryPool::BigObjectMemoryPool(std::size_t capacity) :
	m_main_storage(std::make_unique<BigObject[]>(capacity)),
	m_in_use(capacity, false),
	m_capacity(capacity)
{
	m_free_indices.reserve(capacity);
	
	for (std::size_t index = capacity; index > 0; --index)
	{
		m_free_indices.push_back(index - 1);
	}
	
	if (m_main_storage != nullptr)
	{
		m_begin_address = reinterpret_cast<std::uintptr_t>(m_main_storage.get());
		m_end_address = m_begin_address + capacity * sizeof(BigObject);
	}
}

BigObject* BigObjectMemoryPool::acquire() noexcept
{
	if (m_free_indices.empty())
	{
		return nullptr;
	}
	
	const std::size_t index = m_free_indices.back();
	m_free_indices.pop_back();
	m_in_use[index] = true;
	
	return &m_main_storage[index];
}

void BigObjectMemoryPool::release(BigObject* object)
{
	const std::size_t index = findIndexOf(object);
	
	if (!m_in_use[index])
	{
		throw std::logic_error("BigObjectMemoryPool: object already released");
	}
	
	m_main_storage[index].reset();
	m_in_use[index] = false;
	m_free_indices.push_back(index);
}

std::size_t BigObjectMemoryPool::capacity() const
{
	return m_capacity;
}

std::size_t BigObjectMemoryPool::available() const
{
	return m_free_indices.size();
}

bool BigObjectMemoryPool::ownsBigObject(const BigObject* object) const
{
	if (object == nullptr || m_main_storage == nullptr || m_capacity == 0)
	{
		return false;
	}
	
	const auto address = reinterpret_cast<std::uintptr_t>(object);
	return address >= m_begin_address && address < m_end_address && ((address - m_begin_address) % sizeof(BigObject) == 0);
}

std::size_t BigObjectMemoryPool::findIndexOf(const BigObject* object) const
{
	if (!ownsBigObject(object))
	{
		throw std::invalid_argument("BigObjectMemoryPool: pointer is not owned by this pool");
	}
	
	return static_cast<std::size_t>(object - m_main_storage.get());
}
