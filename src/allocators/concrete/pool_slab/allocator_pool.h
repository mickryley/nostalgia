#pragma once

#include <cstddef> 
#include <new>     
#include <memory>  
#include <cassert>
#include <memory_resource> 

namespace nostalgia::pool {

	// === Base Class ===
	class PoolAllocator {
	public:
		PoolAllocator(std::byte* buf, size_t object_size, size_t object_count, const char* caller = "Unknown");

		void*			allocate();
		void			deallocate(std::byte* ptr);
		void			format(size_t object_size, size_t object_count);
		inline void		rewind() noexcept {} // no-op for pool allocator

	private:
		std::byte*		m_buffer;
		std::byte*		m_head;
	};

	// === Global Static Allocator ===
	extern PoolAllocator g_pool_allocator;

	// === Singleton Static Allocator ===
	struct SingletonPoolAllocator {
		static PoolAllocator& get_instance();
	};

	//static PoolAllocator g_pool_allocator{ reinterpret_cast<std::byte*>(buffer), 16, buffer_size / 16 };
}