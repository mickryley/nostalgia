#pragma once

#include "allocators/stack_lifo/allocator_stack.h"

namespace nostalgia::stack::objects {

	class Vector2D_LocalOverride_StaticAccess {				
		public:
		float x;
		float y;
		Vector2D_LocalOverride_StaticAccess(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
		
		void* operator new(size_t size) {
			return s_stackAllocator.allocate(size);
		}

		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {
			s_stackAllocator.free(ptr);
		}
	};

	class Vector3D_LocalOverride_StaticAccess {
	public:
		float x;
		float y;
		float z;
		Vector3D_LocalOverride_StaticAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
		void* operator new(size_t size) {
			return s_stackAllocator.allocate(size);
		}
		void operator delete(void* ptr) noexcept {
			s_stackAllocator.free(ptr);
		}
	};
	/*
	class Vector2D_LocalOverride_SingletonAccess {
	public:
		float x, y;
		Vector2D_LocalOverride_SingletonAccess(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

		void* operator new(size_t size) {
			return linear::SingletonLinearAllocator::getInstance().allocate(size);
		}

		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {
			//::operator delete(ptr);
			// No need to deallocate
		}
	};

	class Vector3D_LocalOverride_SingletonAccess {
	public:
		float x, y, z;
		Vector3D_LocalOverride_SingletonAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
		void* operator new(size_t size) {
			return linear::SingletonLinearAllocator::getInstance().allocate(size);
		}
		void operator delete(void* ptr) noexcept {
			//::operator delete(ptr);
			// No need to deallocate
		}
	};
	*/
}