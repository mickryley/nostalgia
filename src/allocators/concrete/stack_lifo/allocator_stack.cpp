#include "allocator_stack.h"

#include <cstddef> // For size_t, std::max_align_t
#include <cstdlib> // For posix_memalign

#include <iostream>

#ifdef _MSC_VER
#include <malloc.h> // for _aligned_malloc/_aligned_free
#else
#endif

#include "utils/log.h"

namespace nostalgia::stack {
    
    
    StackAllocator::StackAllocator(char* buf, size_t cap)
        : m_buffer(buf), m_capacity(cap), m_offset(0), m_peakCapacity(0) {
        log::print("StackAllocator: Initialised from location [{}] with a capacity of [{}] bytes", (void*)m_buffer, m_capacity);
        }

    // Alignment-first allocation method
    void* StackAllocator::allocate(size_t bytes){
        size_t alignment = sizeof(StackBlockHeader);
        
        size_t _allocStart = (m_offset + alignment - 1) & ~(alignment - 1);
        size_t _wrappedBytes = bytes + sizeof(StackBlockHeader);

        if (_allocStart + _wrappedBytes > m_capacity) {
#ifdef _DEBUG
			log::print(logFlags::ERROR, "StackAllocator: Out of memory! Requested [{}] bytes, but only [{}] bytes available.", bytes, m_capacity - m_offset);
            log::print(logFlags::ERROR, "StackAllocator: Out of memory! Falling back to malloc.");
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
			log::print("StackAllocator: Allocated [{}] bytes at [{}] (header at [{}], payload at [{}], new offset: [{}], peak: [{}])", 
                bytes, (void*)_ptr, _allocStart, _allocStart + sizeof(StackBlockHeader), m_offset, m_peakCapacity);
#endif
        return _ptr;
    }

    // This method only accepts the top LIFO request
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

        StackBlockHeader* header = reinterpret_cast<StackBlockHeader*>(_ptr);
        size_t blockSize = header->size;

        // Check if the block is at the top of the stack
        // Change from char to byte
        if (static_cast<char*>(_ptr) + blockSize != m_buffer + m_offset) {
            log::print(LogFlags::Error, "StackAllocator: Attempted to free a pointer that is not at the top of the stack, m_offset is [{}]", m_offset);
			log::print(LogFlags::Error, "Comparison Left half is [{} + {}] and Right half is [{} + {}]", (void*)_ptr, blockSize, (void*)m_buffer, m_offset);
            
            return false;
        }
        m_offset -= blockSize;
#ifdef _DEBUG
        log::print("StackAllocator: Freed [{}] bytes at [{}] (new offset: [{}])", header->size, _ptr, m_offset);
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
    }
}