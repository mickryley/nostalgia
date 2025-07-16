#pragma once

#include <cstddef>
#include <new>    
#include <memory> 
#include <cassert>
#include <memory_resource>

namespace nostalgia::freelist {

	// === Base Class ===
	class FreeAllocator {
	public:
		FreeAllocator(std::byte* buf, size_t cap, const char* caller = "Unknown");
		void*		allocate(size_t size);
		void		deallocate(std::byte* ptr);
		void*		coalesce(std::byte* ptr);
		std::byte*	findFirstFit(size_t size);
		std::byte*	findNextFit(size_t size);


		inline void rewind() noexcept {} // no-op for freelist allocator

	private:
		std::byte*	m_buffer;
		std::byte*	m_head;

		size_t		m_capacity;
	};

	// === Global Static Allocator ===
	extern FreeAllocator g_free_allocator;

	// === Singleton Static Allocator ===
	struct SingletonFreeAllocator {
		static FreeAllocator& get_instance();
	};

}