#include "allocator_pool.h"

#include <cstddef> // For size_t, std::max_align_t, std::byte

namespace nostalgia::pool {

	// This method uses a pre-linked list
	PoolAllocator::PoolAllocator(std::byte* buf, size_t objectSize, size_t objectCount)
		: m_buffer(buf) {
		
		for (size_t i = 0; i < objectCount - 1; ++i) {
			std::byte* currentSlot = m_buffer + i * objectSize;
			std::byte* nextSlot = (i < objectCount - 1) ? currentSlot + objectSize : nullptr;
			
			*reinterpret_cast<std::byte**>(currentSlot) = nextSlot;
		}

		m_head = m_buffer;
	};

	void* PoolAllocator::allocate() {
		if (!m_head) return nullptr;

		std::byte* slot = m_head;
		m_head = *reinterpret_cast<std::byte**>(m_head);
		return slot;
	}

	void PoolAllocator::deallocate(std::byte* ptr) {
		std::byte* slot = static_cast<std::byte*>(ptr);
		*reinterpret_cast<std::byte**>(slot) = m_head;
		m_head = slot;
	}

}