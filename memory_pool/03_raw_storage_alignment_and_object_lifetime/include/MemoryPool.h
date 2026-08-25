#pragma once

#include <cstddef>
#include <cstdint>
#include <new>
#include <stdexcept>
#include <vector>

template <typename T>
class MemoryPool
{
public:
	explicit MemoryPool(std::size_t capacity) : 
		m_in_use(capacity, false),
		m_capacity(capacity)
	{
		m_free_indices.reserve(capacity);

		for (std::size_t index = m_capacity; index >0; --index)
		{
			m_free_indices.push_back(index - 1);
		}

		if (0 == m_capacity)
		{
			return;
		}
		
		// Why don't we use:
		//
		// std::make_unique<T[]>(m_capacity)
		//
		// anymore?
		//
		// Because that would construct every T immediately.
		// In this stage we only want raw memory capable of holding T objects.
		// No T object should exist until acquire() is called.
		//
		m_main_storage = static_cast<std::byte*>(
				::operator new ( 
					sizeof(T) * m_capacity, 
					std::align_val_t { alignof(T) } 
					)
				);
	}

	// Should a Memory Pool be copyable?
	//
	// If copid
	// - Who owns the raw storage?
	// - What happens to live onbject state?
	// - What happens to pointers already returned by the pool?
	MemoryPool(const MemoryPool&) = delete;
	MemoryPool& operator=(const MemoryPool&) = delete;

	// Should a Memory Pool be movable?
	//
	// Yes, but it introduces ownership transfer sematics.
	// For now we keep it simple and disable it.
	MemoryPool(const MemoryPool&&) = delete;
	MemoryPool& operator=(const MemoryPool&&) = delete;

	~MemoryPool()
	{
		// What happens if the caller forgets to release an object?
		// 
		// The pool still owns the storage, so any live T objects
		// must be destroyed before the raw memory is freed.
		
		destroyLiveObjects();

		if (m_main_storage != nullptr)
		{
			// Why do we explicitly call aligned ::operator delete here instead of
			// 
			// delete m_main_storage; OR
			// delete[] m_main_storage;
			//
			// Because the storage was allocated directly using aligned ::operator new
			//
			// ::operator new(size, std::align_val_t{alignof(T)})
			// 
			// It was NOT allocated using `new T` or `new T[]`, so using delete or
			// delete[] would be the wrong matching deallocation and would result in
			// undefined behavior.
			//
			// Also, object destruction and storage deallocation are separate here.
			// Individual T objects have already been destroyed explicitly using
			// 
			// object->~T();
			//
			// Now we are only releasing the raw storage owned by the pool.
			// Since we used aligned ::operator new, we must use the matching
			// aligned ::operator delete

			::operator delete (
					m_main_storage,
					std::align_val_t { alignof(T) }
					);
		}
	}


	template <typename... Args>
	[[nodiscard]] T* acquire(Args&&... args) noexcept
	{
		if (m_free_indices.empty())
		{
			return nullptr;
		}

		const std::size_t index = m_free_indices.back();

		std::byte* object_address = memoryAddress(index);
		
		// Does a T object exist at this address yet?
		// No
		//
		// This is only raw storage
		// Placement new begins the lifetime of T at this exact address.
		
		T* object = new (object_address) T (std::forward<Args>(args)...);
		
		// Why do we remove the free index only after constructing T?
		//
		// Because T's constructor may throw
		// If construction fails, the slot should remain available.

		m_free_indices.pop_back();
		m_in_use[index] = true;

		return object;
	}

	void release(T* object)
	{
		const std::size_t index = findIndexOf(object);

		if (!m_in_use[index])
		{
			throw std::logic_error("MemoryPool: object has already been released");
		}
		
		// Why don't we call
		// 
		// object->reset();
		//
		// Because a generic T is not guaranteed to provide reset()
		// 
		// Instead, we end the lifetime of the object completely.
		object->~T();
		
		// The memory is still owned by the pool.
		// 
		// Only the T object's lifetime has ended.
		//
		// This slot is now raw storage again.
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

		const auto begin_address = reinterpret_cast<std::uintptr_t>(m_main_storage);
		const auto end_address = begin_address + (sizeof(T) * m_capacity);
		const auto object_address = reinterpret_cast<std::uintptr_t>(object);
		
		// Why isn't checking only
		//
		// begin <= address < end
		// enough?
		//
		// Because an address could point into the middle of a T slot
		// The modulo check verifies that it points exactly to the start of one of our T sized slots
		return object_address >= begin_address && object_address < end_address && ((object_address - begin_address) % sizeof(T) == 0);
	}

        std::size_t findIndexOf(const T* object) const
	{
		if(!ownsObject(object))
		{
			throw std::invalid_argument("MemoryPool: pointer is not owned by this pool");
		}

		const auto begin_address = reinterpret_cast<std::uintptr_t>(m_main_storage);
		const auto object_address = reinterpret_cast<std::uintptr_t>(object);
		
		// Why can't we simply do
		// object - m_main_storage
		// like we did in the previous pool?
		//
		// Previously, m_main_storage was T[] storage, so both pointers were T*
		// and pointer subtraction directly gave us the element index.
		// 
		// Now m_main_storage represents raw byte storage while object is a T*.
		// We therefore calculate the byte distance from the beginning of the
		// pool and divide by sizeof(T) to determine which slot it belongs to.
		return static_cast<std::size_t>((object_address - begin_address) / sizeof(T));
	}

	[[nodiscard]] std::byte* memoryAddress(std::size_t index) noexcept
	{
		// Why use std::byte* here?
		// 
		// Pointer arithmetic on std::byte advances one byte at a time,
		// so calculating slot offsets is explicit and natural.

		return m_main_storage + (index * sizeof(T));
	}	
	
	// Why is destroyLiveObjects() noexcept?
	//
	// It is called during MemoryPool destruction.
	// Cleanup code should not allow exceptions to escape, especially during
	// stack unwinding, where another escaping exception can cause std::terminate().
	// 
	// This also assumes that T's destructor follows the normal C++ convention
	// of not throwing exceptions.
	void destroyLiveObjects() noexcept
	{
		for (std::size_t index  = 0; index < m_capacity; ++index)
		{
			if (!m_in_use[index])
			{
				continue;
			}
			
			// Why is reinterpret_cast used here?
			// 
			// The pool stores raw bytes, but m_in_use tells us
			// that a live T currently exists at this slot.

			T* object = reinterpret_cast<T*>(memoryAddress(index));

			object->~T();
		}
	}
	
	// Raw storage only
	//
	// This is not an array of already constructed T objects.
        std::byte* m_main_storage{nullptr};

	// Aren't m_free_indices and m_in_use storing duplicate information?
	// 
	// To some extent, yes
	// 
	// m_free_indices gives acquire() an O(1) way to find a free slot
	// without scanning the entire pool
	// 
	// m_in_use lets us quickly validate release() and identify which
	// slots currently contain live T objects during pool destruction.
	//
	// This intentionally trades some duplicated state for simpler,
	// faster operations. 
	// A more advanced pool could encode the free/live
	// state directly into its slot metadata or free list structure.

	// Keeps acquire() free from scanning the entire pool
        std::vector<std::size_t> m_free_indices;

	// Tracks whether a live T currently exists in each slot
        std::vector<bool> m_in_use;
        
	std::size_t m_capacity{};
};
