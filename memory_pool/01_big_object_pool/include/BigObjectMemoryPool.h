#pragma once

# include "BigObject.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

class BigObjectMemoryPool
{
public:
	explicit BigObjectMemoryPool(std::size_t capacity);
	
	// Why are the copy constructor and copy assignment operator deleted?
	// What problems could arise if two memory pools owned the same storage?
	// I would love to hear from you before we explore it in a future stage
	BigObjectMemoryPool(const BigObjectMemoryPool&) = delete;
	BigObjectMemoryPool& operator=(const BigObjectMemoryPool&) = delete;

	// Why do you think this function is marked as [[nodiscard]]?
	// Share your reasoning in the GitHub comments
	[[nodiscard]] BigObject* acquire() noexcept;
	void release (BigObject* big_object);

	std::size_t capacity() const;
	std::size_t available() const;
private:
	bool ownsBigObject(const BigObject* big_object) const;
	std::size_t findIndexOf(const BigObject* big_object) const;

	std::unique_ptr<BigObject[]> m_main_storage;
	std::vector<std::size_t> m_free_indices;
	std::vector<bool> m_in_use;
	std::size_t m_capacity{};
	std::uintptr_t m_begin_address{};
	std::uintptr_t m_end_address{};
};



