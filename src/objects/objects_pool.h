#pragma once

#include "allocators/pool_slab/allocator_pool.h"

namespace nostalgia::pool::objects {

	class Vector2D_LocalOverride_StaticAccess {				
		public:
		float x;
		float y;
		Vector2D_LocalOverride_StaticAccess(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
		
		void* operator new(size_t size) {
			(void)size; // Fixed size, suppress warning while remaining compatible with C++ standards
			return s_poolAllocator.allocate();
		}

		inline void* operator new(size_t, void* ptr) noexcept { return ptr; }

		void operator delete(void* ptr) noexcept {
			s_poolAllocator.deallocate(reinterpret_cast<std::byte*>(ptr));
		}
	};

	class Vector3D_LocalOverride_StaticAccess {
	public:
		float x;
		float y;
		float z;
		Vector3D_LocalOverride_StaticAccess(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
		void* operator new(size_t size) {
			(void)size; // Fixed size, suppress warning while remaining compatible with C++ standards
			return s_poolAllocator.allocate();
		}
		void operator delete(void* ptr) noexcept {
			s_poolAllocator.deallocate(reinterpret_cast<std::byte*>(ptr));
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