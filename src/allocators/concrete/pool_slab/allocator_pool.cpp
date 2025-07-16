#include "allocator_pool.h"

#include <cstddef> // For size_t, std::max_align_t, std::byte

#include "utils/log.h"

namespace nostalgia::pool {

	namespace {
		// Launch Buffer
		constexpr size_t buffer_size = 1024 * 1024;				// 1 MB
		size_t fixed_object_size = sizeof(std::max_align_t) * 2;	// Use max_align_t for alignment
		std::byte* buffer = new std::byte[buffer_size];			// Mallocate a buffer of 1 MB
	}

	// === Global Static Allocator ===
	PoolAllocator g_pool_allocator(buffer, fixed_object_size, buffer_size / fixed_object_size, "Global");

	// === Singleton Static Allocator ===
	PoolAllocator& SingletonPoolAllocator::get_instance() {
		static PoolAllocator s_pool_allocator(buffer, fixed_object_size, buffer_size / fixed_object_size, "Singleton Static");
		return s_pool_allocator;
	}

    // === Base Class Constructors ===
	PoolAllocator::PoolAllocator(std::byte* buf, size_t object_size, size_t object_count, const char* caller)
		: m_buffer(buf) {

		log::print("[{}] PoolAllocator: Initialised from location [{}] with a capacity of [{}] bytes for [{}] objects of [{}] size.", 
			caller, (void*)m_buffer, (object_size * object_count), object_count, object_size);
		format(object_size, object_count);
		m_head = m_buffer;
	};

	void PoolAllocator::format(size_t object_size, size_t object_count) {
		for (size_t i = 0; i < object_count - 1; ++i) {
			std::byte* currentSlot = m_buffer + i * object_size;
			std::byte* nextSlot = (i < object_count - 1) ? currentSlot + object_size : nullptr;

			*reinterpret_cast<std::byte**>(currentSlot) = nextSlot;
		}
	}

	void* PoolAllocator::allocate() {
		if (!m_head) return nullptr;

		std::byte* slot = m_head;
		m_head = *reinterpret_cast<std::byte**>(m_head);
		return slot;
	}

	void* PoolAllocator::allocate(size_t bytes) {
		if (bytes > fixed_object_size) {
			log::print(LogFlags::Error, "PoolAllocator: Requested size [{}] exceeds fixed object size [{}]. Allocation failed.", bytes, fixed_object_size);
			return nullptr;
		}
		return allocate();
	}

	void PoolAllocator::deallocate(std::byte* ptr) {
		std::byte* slot = static_cast<std::byte*>(ptr);
		*reinterpret_cast<std::byte**>(slot) = m_head;
		m_head = slot;
	}

}