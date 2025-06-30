#include "allocator_linear.h"

#include <iostream>
#include <stdlib.h>

#ifdef _MSC_VER
#include <malloc.h> // for _aligned_malloc/_aligned_free
#endif


#include "log.h"

namespace nostalgia::linear {

        // Constructor: Initializes the allocator with a pre-allocated buffer and its capacity
        LinearAllocator::LinearAllocator(char* buf, size_t cap) : m_buffer(buf), m_capacity(cap), m_offset(0), m_peakCapacity(0) {
        log::print("LinearAllocator: Initialised from location [{}] with a capacity of [{}] bytes", (void*)m_buffer, m_capacity);
        }

        // Alignment-first allocation method
        void* LinearAllocator::allocate(size_t bytes, size_t alignment) {

            size_t _allocStart = (m_offset + alignment - 1) & ~(alignment - 1);
			
            if (_allocStart + bytes > m_capacity) {
#ifdef _DEBUG
				//log::print(logFlags::ERROR, "LinearAllocator: Out of memory! Requested [{}] bytes, but only [{}] bytes available.", bytes, m_capacity - m_offset);
                //log::print(logFlags::ERROR, "LinearAllocator: Out of memory! Falling back to malloc.");
#endif
                void* fallback = _aligned_malloc(bytes, alignment);
                if (!fallback) {
                    throw std::bad_alloc();
                }

#ifdef _DEBUG
				//log::print("LinearAllocator: Fallback allocation at [{}] for [{}] bytes", (void*)fallback, bytes);
#endif

                return fallback;
            }

            void* _ptr = m_buffer + _allocStart;

            m_offset = _allocStart + bytes;

            m_peakCapacity = std::max(m_peakCapacity, m_offset);

#ifdef _DEBUG
			//log::print("LinearAllocator: Allocated [{}] bytes at [{}] (offset: [{}], peak: [{}])", bytes, (void*)_ptr, m_offset, m_peakCapacity);
#endif

            return _ptr;
        }

        void LinearAllocator::rewind() {
            m_offset = 0;
        }
}