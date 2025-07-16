#pragma once

#include "allocators/concrete/linear_bump/allocator_linear.h"

namespace nostalgia::linear::objects {

	// === Override, Global Static Access ===
	class Vector2D_LocalOverride_StaticAccess {				
		public:
		float x;
		float y;
		Vector2D_LocalOverride_StaticAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y) {
			(void)z; 
		}
		void* operator new(size_t size) {	return g_linear_allocator.allocate(size);	}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept { (void)ptr; };
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};

	class Vector3D_LocalOverride_StaticAccess {
	public:
		float x;
		float y;
		float z;
		Vector3D_LocalOverride_StaticAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
		void* operator new(size_t size) {	return g_linear_allocator.allocate(size);	}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept { (void)ptr; };
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};
	// === Override, Singleton Static Access ===
	class Vector2D_LocalOverride_SingletonAccess {
	public:
		float x, y;
		Vector2D_LocalOverride_SingletonAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y) {
			(void)z;
		}

		void* operator new(size_t size) {	return linear::SingletonLinearAllocator::get_instance().allocate(size);	}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept { (void)ptr; };
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};

	class Vector3D_LocalOverride_SingletonAccess {
	public:
		float x, y, z;
		Vector3D_LocalOverride_SingletonAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
		void* operator new(size_t size) { return linear::SingletonLinearAllocator::get_instance().allocate(size); }
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept { (void)ptr; };
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};
}

static_assert(true, "End of linear objects header");