#include "allocator_linear.h"

#include <iostream>


namespace nostalgia::linear {

        // Constructor: Initializes the allocator with a pre-allocated buffer and its capacity
        LinearAllocator::LinearAllocator(char* buf, size_t cap) : m_buffer(buf), m_capacity(cap), m_offset(0), m_peakCapacity(0) {
        std::cout << "LinearAllocator: Initialized with buffer at " << (void*)m_buffer << " and capacity " << m_capacity << std::endl;
        }

        // Alignment-first allocation method
        void* LinearAllocator::allocate(size_t bytes, size_t alignment) {

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

        void LinearAllocator::rewind() {
            m_offset = 0;
        }
}