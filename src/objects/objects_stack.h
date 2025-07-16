#pragma once

#include "allocators/concrete/stack_lifo/allocator_stack.h"

namespace nostalgia::stack::objects {

	class Vector2D_LocalOverride_StaticAccess {				
		public:
		float x;
		float y;
		Vector2D_LocalOverride_StaticAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y) {
			(void)z; 
		}
		
		void* operator new(size_t size) {	return g_stack_allocator.allocate(size);	}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {	g_stack_allocator.free(ptr);	}
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};

	class Vector3D_LocalOverride_StaticAccess {
	public:
		float x;
		float y;
		float z;
		Vector3D_LocalOverride_StaticAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
		
		void* operator new(size_t size) {	return g_stack_allocator.allocate(size);	}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {	g_stack_allocator.free(ptr);	}
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};

	class Vector2D_LocalOverride_SingletonAccess {
	public:
		float x;
		float y;
		Vector2D_LocalOverride_SingletonAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y) {
			(void)z;
		}

		void* operator new(size_t size) {	return stack::SingletonStackAllocator::get_instance().allocate(size);	}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept { stack::SingletonStackAllocator::get_instance().free(ptr);	}
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};

	class Vector3D_LocalOverride_SingletonAccess {
	public:
		float x;
		float y;
		float z;
		Vector3D_LocalOverride_SingletonAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
		void* operator new(size_t size) {	return stack::SingletonStackAllocator::get_instance().allocate(size);	}
		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept { stack::SingletonStackAllocator::get_instance().free(ptr);	}
		void operator delete(void* ptr, void*) noexcept { (void)ptr; }
	};
}

static_assert(true, "End of stack objects header");