#pragma once

#include "allocators/concrete/pool_slab/allocator_pool.h"

namespace nostalgia::pool::objects {

	class Vector2D_LocalOverride_StaticAccess {				
		public:
		float x;
		float y;
		Vector2D_LocalOverride_StaticAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y) {
			(void)z; 
		}

		void* operator new(size_t size) {
			(void)size; // Fixed size - no check implementation
			return g_pool_allocator.allocate();
		}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {
			g_pool_allocator.deallocate(reinterpret_cast<std::byte*>(ptr));
		}
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};

	class Vector3D_LocalOverride_StaticAccess {
	public:
		float x;
		float y;
		float z;
		Vector3D_LocalOverride_StaticAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

		void* operator new(size_t size) {
			(void)size; // Fixed size, suppress warning while remaining compatible with C++ standards
			return g_pool_allocator.allocate();
		}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {
			g_pool_allocator.deallocate(reinterpret_cast<std::byte*>(ptr));
		}
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};
	class Vector2D_LocalOverride_SingletonAccess {
	public:
		float x;
		float y;
		Vector2D_LocalOverride_SingletonAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y) {
			(void)z;
		}

		void* operator new(size_t size) {
			(void)size; // Fixed size - no check implementation
			return pool::SingletonPoolAllocator::get_instance().allocate();
		}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {
			pool::SingletonPoolAllocator::get_instance().deallocate(reinterpret_cast<std::byte*>(ptr));
		}
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};

	class Vector3D_LocalOverride_SingletonAccess {
	public:
		float x;
		float y;
		float z;
		Vector3D_LocalOverride_SingletonAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

		void* operator new(size_t size) {
			(void)size; // Fixed size, suppress warning while remaining compatible with C++ standards
			return pool::SingletonPoolAllocator::get_instance().allocate();
		}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {
			pool::SingletonPoolAllocator::get_instance().deallocate(reinterpret_cast<std::byte*>(ptr));
		}
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};
}

static_assert(true, "End of pool objects header");