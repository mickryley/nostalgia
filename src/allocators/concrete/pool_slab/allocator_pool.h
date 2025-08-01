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
		void*			allocate(size_t bytes);
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
	// === Template Allocator ===
	template <typename T>
	class PoolAllocatorTemplate {
	public:
		using value_type = T;
		PoolAllocatorTemplate() = default;
		template <typename U>
		PoolAllocatorTemplate(const PoolAllocatorTemplate<U>&) {}
		T* allocate(std::size_t n) {
			return static_cast<T*>(g_pool_allocator.allocate(n * sizeof(T)));
		}
		template<typename... Args>
		T* create(Args&&... args) {
			void* mem = allocate(1); // one T-sized block
			return new (mem) T(std::forward<Args>(args)...);
		}
		void destroy(T* p) {
			if (p) p->~T(); 
		}
		inline void		rewind() noexcept {} // no-op for pool allocator
		void deallocate(T* ptr, std::size_t n) {
			(void) n;
			g_pool_allocator.deallocate(reinterpret_cast<std::byte*>(ptr));
		}
	};
}
