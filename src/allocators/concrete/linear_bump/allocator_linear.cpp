#include "allocator_linear.h"

#include <iostream>
//#include <stdlib.h>
#include <cstdlib>

#ifdef _MSC_VER
#include <malloc.h> // for _aligned_malloc/_aligned_free
#endif

#include "utils/log.h"

namespace nostalgia::linear {

        LinearAllocator::LinearAllocator(char* buf, size_t cap) 
        : m_buffer(buf), m_capacity(cap), m_offset(0), m_peakCapacity(0) {
        log::print("LinearAllocator: Initialised from location [{}] with a capacity of [{}] bytes", (void*)m_buffer, m_capacity);
        }

        // This is an alignment-first allocation method - consider adding alternative patterns
        void* LinearAllocator::allocate(size_t bytes, size_t alignment) {

            size_t _allocStart = (m_offset + alignment - 1) & ~(alignment - 1);
			
            if (_allocStart + bytes > m_capacity) {
#ifdef _DEBUG
				//log::print(logFlags::ERROR, "LinearAllocator: Out of memory! Requested [{}] bytes, but only [{}] bytes available.", bytes, m_capacity - m_offset);
                //log::print(logFlags::ERROR, "LinearAllocator: Out of memory! Falling back to malloc.");
#endif
                // Graceful Fallback solution - currently windows only implementation
                void* fallback = nullptr;
#ifdef _MSC_VER
                fallback = _aligned_malloc(bytes, alignment);
#else
         //       fallback = std::aligned_alloc(alignment, bytes);
                // alignment must be a power of two and multiple of `sizeof(void*)`
                if (posix_memalign(&fallback, alignment, bytes) != 0) {
                    fallback = nullptr;
                }
#endif
                //void* fallback = _aligned_malloc(bytes, alignment);
                
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

        /*
        LinearMemoryResource::LinearMemoryResource(void* buf, std::size_t cap)
        : m_buffer(static_cast<char*>(buf)), m_capacity(cap) {}


        void* LinearMemoryResource::do_allocate(std::size_t bytes, std::size_t alignment) {
            std::size_t aligned = (reinterpret_cast<std::uintptr_t>(m_buffer + m_offset) + (alignment - 1)) & ~(alignment - 1);
            void* ptr = reinterpret_cast<void*>(aligned);
            m_offset = (aligned + bytes) - reinterpret_cast<std::uintptr_t>(m_buffer);
            if (m_offset > m_capacity) throw std::bad_alloc();
            return ptr;
        }

        void LinearMemoryResource::do_deallocate(void*, std::size_t, std::size_t) {
            // No-op in linear allocators
        }

        bool LinearMemoryResource::do_is_equal(const memory_resource& other) const noexcept {
            return this == &other;
        }*/
}