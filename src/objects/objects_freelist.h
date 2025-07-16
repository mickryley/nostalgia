#pragma once

#include "allocators/concrete/freelist_variable/allocator_freelist.h"

namespace nostalgia::freelist::objects {

	// === Override, Global Static Access ===
	class Vector2D_LocalOverride_StaticAccess {				
		public:
		float x;
		float y;
		Vector2D_LocalOverride_StaticAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y) {
			(void)z; 
		}

		void* operator new(size_t size) {	return g_free_allocator.allocate(size);	}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {
			g_free_allocator.deallocate(reinterpret_cast<std::byte*>(ptr));
		}
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};

	class Vector3D_LocalOverride_StaticAccess {
	public:
		float x;
		float y;
		float z;
		Vector3D_LocalOverride_StaticAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
		void* operator new(size_t size) {	return g_free_allocator.allocate(size);	}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {
			g_free_allocator.deallocate(reinterpret_cast<std::byte*>(ptr));
		}
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};

	// === Override, Singleton Access ===
	class Vector2D_LocalOverride_SingletonAccess {
	public:
		float x;
		float y;
		Vector2D_LocalOverride_SingletonAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y) {
			(void)z;
		}
		void* operator new(size_t size) {	return nostalgia::freelist::SingletonFreeAllocator::get_instance().allocate(size);	}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {
			freelist::SingletonFreeAllocator::get_instance().deallocate(reinterpret_cast<std::byte*>(ptr));
		}
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};

	class Vector3D_LocalOverride_SingletonAccess {
	public:
		float x;
		float y;
		float z;
		Vector3D_LocalOverride_SingletonAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
		void* operator new(size_t size) {	return nostalgia::freelist::SingletonFreeAllocator::get_instance().allocate(size);	}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {
			freelist::SingletonFreeAllocator::get_instance().deallocate(reinterpret_cast<std::byte*>(ptr));
		}
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};
}


static_assert(true, "End of freelist objects header");