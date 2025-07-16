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
		void*			allocate();
		void			deallocate(std::byte* ptr);
		inline void		rewind() noexcept {} 

	private:
		std::byte*		m_buffer;
		std::byte*		m_head;
	};

	static DefaultAllocator g_default_allocator{};
}