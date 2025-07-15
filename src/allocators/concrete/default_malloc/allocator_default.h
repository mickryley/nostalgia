#pragma once

#include <cstddef> 
#include <new>     
#include <memory>  
#include <cassert>
#include <memory_resource> 

namespace nostalgia::default_malloc {

	class DefaultAllocator {
	public:
		DefaultAllocator() = default;
		void* allocate();
		void deallocate(std::byte* ptr);
		inline void rewind() noexcept {} // no-op for pool allocator

	private:
		std::byte* m_buffer;
		std::byte* m_head;
		//size_t m_objectSize;
		//size_t m_objectCount;
		//size_t m_offset = 0;

		//size_t m_peakCapacity = 0;
		//size_t m_peakOffset = 0;
	};

	static DefaultAllocator s_default_allocator{};
}