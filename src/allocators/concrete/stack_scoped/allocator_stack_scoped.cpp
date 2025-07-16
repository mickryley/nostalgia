#include "allocator_stack_scoped.h"

#include <cstddef> // For size_t, std::max_align_t
#include <cstdlib> // For posix_memalign

#include <iostream>

#ifdef _MSC_VER
#include <malloc.h> // for _aligned_malloc/_aligned_free
#else
#endif

#include "utils/log.h"

namespace nostalgia::stack::scoped {

    struct StackBlockHeader {
        uint32_t size; // True size / offset including the header
    };

    namespace {
        // Launch Buffer
        constexpr size_t buffer_size = 1024 * 1024;     // 1 MB
        std::byte* buffer = new std::byte[buffer_size]; // Mallocate a buffer of 1 MB
    }

    // === Global Static Allocator ===
    StackAllocator g_stack_allocator(buffer, buffer_size, "Global");

    // === Singleton Static Allocator ===
    StackAllocator& SingletonStackAllocator::get_instance() {
        static StackAllocator s_stack_allocator(buffer, buffer_size, "Singleton Static");
        return s_stack_allocator;
    }

    // === Base Class Constructors ===
    StackAllocator::StackAllocator(std::byte* buf, size_t cap, const char* caller)
        : m_buffer(buf), m_capacity(cap), m_offset(0), m_peakCapacity(0) {
        log::print("[{}] Scoped Stack Allocator: Initialised from location [{}] with a capacity of [{}] bytes", caller, (void*)m_buffer, m_capacity);
    }


    // Alignment-first allocation method
    void* StackAllocator::allocate(size_t bytes){
        size_t alignment = sizeof(StackBlockHeader);
        
        size_t _allocStart = (m_offset + alignment - 1) & ~(alignment - 1);
        size_t _wrappedBytes = bytes + sizeof(StackBlockHeader);

        if (_allocStart + _wrappedBytes > m_capacity) {
#ifdef _DEBUG
			//log::print(LogFlags::Error, "StackAllocator: Out of memory! Requested [{}] bytes, but only [{}] bytes available.", bytes, m_capacity - m_offset);
            //log::print(LogFlags::Error, "StackAllocator: Out of memory! Falling back to malloc.");
#endif
            void* fallback = nullptr;
#ifdef _MSC_VER
                fallback = _aligned_malloc(_wrappedBytes, alignment);
#else
            if (posix_memalign(&fallback, alignment, _wrappedBytes) != 0) {
                fallback = nullptr;
            }
#endif
            if (!fallback) {
                    throw std::bad_alloc();
            }
            return fallback;
        }

        void* _ptr = m_buffer + _allocStart;
        reinterpret_cast<StackBlockHeader*>(_ptr)->size = static_cast<uint32_t>(_wrappedBytes);

        _ptr = static_cast <std::byte*>(_ptr) + sizeof(StackBlockHeader);

        m_offset = _allocStart + _wrappedBytes;

        m_peakCapacity = std::max(m_peakCapacity, m_offset);
#ifdef _DEBUG
			//log::print("StackAllocator: Allocated [{}] bytes at [{}] (header at [{}], payload at [{}], new offset: [{}], peak: [{}])", 
           //     bytes, (void*)_ptr, _allocStart, _allocStart + sizeof(StackBlockHeader), m_offset, m_peakCapacity);
#endif
        return _ptr;
    }

    // Scoped ignored LIFO, and will free anything from a valid pointer
    bool StackAllocator::free(void* ptr) noexcept {

#ifdef _DEBUG
        log::print("Size of header (free): [{}]", sizeof(StackBlockHeader));
#endif
        if (ptr == nullptr) return false;
        
        // Correct the _ptr to the header location
        void* _ptr = static_cast <std::byte*>(ptr) - sizeof(StackBlockHeader);
        
        if(_ptr < m_buffer || _ptr >= m_buffer + m_capacity) {
            log::print(LogFlags::Error, "StackAllocator: Attempted to free a pointer outside the allocated buffer range.");
            return false;
		}
        
		//m_offset = static_cast<std::byte*>(_ptr) - m_buffer;

#ifdef _DEBUG
		log::print("StackAllocator: Freed memory at [{}], new offset: [{}]", (void*)_ptr, m_offset);
#endif
		return true;
    }    

    void StackAllocator::reset() noexcept {
        m_offset = 0;
        m_peakCapacity = 0;
        log::print("StackAllocator: Reset allocator to initial state (offset: [{}], peak: [{}])", m_offset, m_peakCapacity);
	}

    void StackAllocator::rewind() noexcept {
        m_offset = 0;
        //log::print("StackAllocator: Rewind allocator to initial state (offset: [{}])", m_offset);
    }
}