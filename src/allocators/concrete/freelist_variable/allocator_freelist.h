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

	// === Template Allocator ===
	template <typename T>
	class FreeAllocatorTemplate {
	public:
		using value_type = T;
		FreeAllocatorTemplate() = default;
		template <typename U>
		FreeAllocatorTemplate(const FreeAllocatorTemplate<U>&) {}
		
		T* allocate(std::size_t n) {
			return static_cast<T*>(g_free_allocator.allocate(n * sizeof(T)));
		}
		template<typename... Args>
		T* create(Args&&... args) {
			void* mem = allocate(1); // one T-sized block
			return new (mem) T(std::forward<Args>(args)...);
		}
		void destroy(T* p) {
			if (p) p->~T(); 
		}
		void deallocate(T* p, std::size_t n) noexcept {
			(void)n; // No-op: handled by allocator reset
			g_free_allocator.deallocate(reinterpret_cast<std::byte*>(p));
		}
		void rewind() {
			g_free_allocator.rewind();
		}
	};

	// === Template Allocator - Singleton Access ===
	template <typename T>
	class FreeAllocatorTemplateSingletonAccess {
	public:
		using value_type = T;
		FreeAllocatorTemplateSingletonAccess() = default;
		template <typename U>
		FreeAllocatorTemplateSingletonAccess(const FreeAllocatorTemplateSingletonAccess<U>&) {}

		T* allocate(std::size_t n) {
			return static_cast<T*>(SingletonFreeAllocator::get_instance().allocate(n * sizeof(T)));
		}
		template<typename... Args>
		T* create(Args&&... args) {
			void* mem = allocate(1); // one T-sized block
			return new (mem) T(std::forward<Args>(args)...);
		}
		void destroy(T* p) {
			if (p) p->~T(); 
		}
		void deallocate(T* p, std::size_t n) noexcept {
			(void)n; // No-op: handled by allocator reset
			SingletonFreeAllocator::get_instance().deallocate(reinterpret_cast<std::byte*>(p));
		}
		void rewind() {
			SingletonFreeAllocator::get_instance().rewind();
		}
	};

	// === Template Allocator - Cached Singleton Access ===
	template <typename T>
	class FreeAllocatorTemplateCachedSingletonAccess {
	private:
		FreeAllocator* cached_allocator = nullptr;
	public:
		using value_type = T;
		FreeAllocatorTemplateCachedSingletonAccess()
		: cached_allocator(&SingletonFreeAllocator::get_instance()) {}

		template <typename U>
		FreeAllocatorTemplateCachedSingletonAccess(const FreeAllocatorTemplateCachedSingletonAccess<U>&)
				: cached_allocator(&SingletonFreeAllocator::get_instance()) {}

		T* allocate(std::size_t n) {
			return static_cast<T*>(cached_allocator->allocate(n * sizeof(T)));
		}
		template<typename... Args>
		T* create(Args&&... args) {
			void* mem = allocate(1); // one T-sized block
			return new (mem) T(std::forward<Args>(args)...);
		}
		void destroy(T* p) {
			if (p) p->~T();
		}
		void deallocate(T* p, std::size_t n) noexcept {
			(void)n;
			cached_allocator->deallocate(reinterpret_cast<std::byte*>(p));
		}
		void rewind() {
			cached_allocator->rewind();
		}
	};

}	