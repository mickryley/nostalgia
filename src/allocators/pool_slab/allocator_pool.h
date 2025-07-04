#pragma once

#include <cstddef> // For size_t, std::max_align_t, std::byte
#include <new>     // For std::bad_alloc
#include <memory>  // For alignof
#include <cassert>
#include <memory_resource> // For pmr

namespace nostalgia::pool {

	namespace {
		constexpr size_t bufferSize = 1024 * 1024; // 1 MB
		char* buffer = new char[bufferSize]; // Mallocate a buffer of 1 MB
	}

	class PoolAllocator {
	public:
		PoolAllocator(std::byte* buf, size_t objectSize, size_t objectCount);
		void* allocate();
		void deallocate(std::byte* ptr);
		inline void rewind() noexcept {} // no-op for pool allocator

	private:
		std::byte* m_buffer;
		std::byte* m_head;
		size_t m_objectSize;
		size_t m_objectCount;
		size_t m_offset = 0;

		size_t m_peakCapacity = 0;
		size_t m_peakOffset = 0;
	};

	static PoolAllocator s_poolAllocator{ reinterpret_cast<std::byte*>(buffer), 16, bufferSize / 16 };
}