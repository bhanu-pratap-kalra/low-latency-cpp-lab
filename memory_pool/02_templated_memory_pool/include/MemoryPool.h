#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

//Design Question
//
//Why do we have implementation in header?
//Can it cause multiple definition linkingerror?

template <typename T>
class MemoryPool
{
public:
	explicit MemoryPool(std::size_t capacity) : 
		m_main_storage (capacity == 0 ? nullptr : std::make_unique<T[]>(capacity)),
		m_in_use(capacity, false),
		m_capacity(capacity)
	{
		m_free_indices.reserve(capacity);

		for (std::size_t index = m_capacity; index >0; --index)
		{
			m_free_indices.push_back(index - 1);
		}

		if (m_main_storage != nullptr)
		{
			m_begin_address = reinterpret_cast<std::uintptr_t>(m_main_storage.get());
			m_end_address = reinterpret_cast<std::uintptr_t>(m_begin_address + (sizeof(T) * m_capacity));
		}
	}

	//Design Question
	//
	//Should a Memory Pool be copyable?
	//
	MemoryPool(const MemoryPool&) = delete;
	MemoryPool& operator=(const MemoryPool&) = delete;


	//Design Question
	//
	//Why do we mark acquire [[nodiscard]] and noexcept?
	//
	[[nodiscard]] T* acquire() noexcept
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

	//Design Question
	//
	//Why does release() validate that the object belong to the pool?
	//
	void release(T* object)
	{
		const std::size_t index = findIndexOf(object);

		//Why do we track, if index is currently in use?
		//What could happen, if the same object is released twice?
		if (!m_in_use[index])
		{
			throw std::logic_error("MemoryPool: object has already been released");
		}
		
		//Why dont we call object.reset() here like BigObject?
		//How will we reset or clean the object here?
		m_in_use[index] = false;
		m_free_indices.push_back(index);
	}

	std::size_t capacity() const
	{
		return m_capacity;
	}

	std::size_t available() const
	{
		return m_free_indices.size();
	}

private:
	bool ownsObject(const T* object) const
	{
		if (object == nullptr || m_main_storage == nullptr || m_capacity == 0)
		{
			return false;
		}

		const auto address = reinterpret_cast<std::uintptr_t>(object);
		return address >= m_begin_address && address < m_end_address && ((address - m_begin_address) % sizeof(T) == 0);
	}

        std::size_t findIndexOf(const T* object) const
	{
		if(!ownsObject(object))
		{
			throw std::invalid_argument("MemoryPool: pointer is not owned by this pool");
		}

		//Why can subtracting two T* pointers gives us array index here?
		return static_cast<std::size_t>(object - m_main_storage.get());
	}

	//What ownership problem does unique_ptr solve here?
	//Why not std::vector<T>?
        std::unique_ptr<T[]> m_main_storage;

	//Why do we maintain free indices list, why don't we simply check m_in_use?
        std::vector<std::size_t> m_free_indices;
        std::vector<bool> m_in_use;
        std::size_t m_capacity{};

	//Is chaching these addresses worth?
        std::uintptr_t m_begin_address{};
        std::uintptr_t m_end_address{};
};


