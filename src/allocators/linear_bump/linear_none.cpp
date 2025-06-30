#include "allocator_linear.h"

#include <cstddef> // For size_t, std::max_align_t
#include <new>     // For std::bad_alloc
#include <memory>  // For alignof

#include <iostream>

namespace nostalgia::linear {

    namespace none {
        
    }

    // No (Default) Implementation
    void linearAllocator(){
        // Allocate
        // Free
        // Rewind / Reset
     }
    /*
    class LinearAllocator {
    public:
        // Constructor: Initializes the allocator with a pre-allocated buffer and its capacity
        LinearAllocator(char* buf, size_t cap) : m_buffer(buf), m_capacity(cap), m_offset(0) {}

        // Alignment-first allocation method
        void* allocate(size_t bytes, size_t alignment = alignof(std::max_align_t)) {
            
            size_t _allocStart = (m_offset + alignment - 1) & ~(alignment - 1);

            // Check if the allocation would exceed the buffer's capacity
            if (_allocStart + bytes > m_capacity) {
				std::cerr << "LinearAllocator: Out of memory!" << std::endl;
                throw std::bad_alloc(); // Indicate out-of-memory
            }

            void* _ptr = m_buffer + _allocStart;

            m_offset = _allocStart + bytes;

			m_peakCapacity = std::max(m_peakCapacity, m_offset);

#ifdef _DEBUG
            std::cout << "A" << "\t@C " << (void*)_allocStart << "\t@R " << (void*)_ptr << "\tO " << m_offset << "\tP " << bytes << std::endl;
#endif

            return _ptr;
        }

        void rewind() {
            m_offset = 0;
        }

    private:
        char* m_buffer;      // Pointer to the start of the memory chunk
        size_t m_capacity;   // Total size of the memory chunk
        size_t m_offset;     // Current position for the next allocation

		size_t m_peakCapacity = 0; // Peak capacity used during allocations
    };
    */
}