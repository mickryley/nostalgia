#pragma once

#include <cstddef> // For size_t, std::max_align_t, std::byte
#include <new>     // For std::bad_alloc
#include <memory>  // For alignof
#include <cassert>
#include <memory_resource> // For pmr

namespace nostalgia::freelist {

	namespace {
		constexpr size_t bufferSize = 1024 * 1024; // 1 MB
		char* buffer = new char[bufferSize]; // Mallocate a buffer of 1 MB
	}

	class FreeAllocator {
	public:
		FreeAllocator(std::byte* buf, size_t cap);
		void* allocate(size_t size);
		void deallocate(std::byte* ptr);
		void* coalesce(std::byte* ptr);
		std::byte* findFirstFit(size_t size);
		std::byte* findNextFit(size_t size);


		inline void rewind() noexcept {} // no-op for freelist allocator

	private:
		std::byte* m_buffer;
		std::byte* m_head;

		//std::byte* m_maxAddress;
		//size_t m_allocCount;
		size_t m_cap;

		//size_t m_objectSize;
		//size_t m_objectCount;
		//size_t m_offset = 0;

		//size_t m_peakCapacity = 0;
		//size_t m_peakOffset = 0;
	};

	static FreeAllocator s_freeAllocator{ reinterpret_cast<std::byte*>(buffer), bufferSize };
}