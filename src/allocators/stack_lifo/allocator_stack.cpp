#include <cstddef> // For size_t, std::max_align_t

#ifdef _MSC_VER
#include <malloc.h> // for _aligned_malloc/_aligned_free
#else
#include <cstdlib> // For posix_memalign
#endif

#include "log.h"

namespace nostalgia::stack {
    class StackAllocator{
    private:
    char* m_buffer;
    size_t m_capacity;
    size_t m_offset = 0;

    // Logging only
    size_t m_peakCapacity = 0; 
    public:
    StackAllocator(char* buf, size_t cap) 
        : m_buffer(buf), m_capacity(cap), m_offset(0), m_peakCapacity(0) {
        log::print("LinearAllocator: Initialised from location [{}] with a capacity of [{}] bytes", (void*)m_buffer, m_capacity);
        }

    void* allocate(size_t bytes, size_t alignment = alignof(std::max_align_t)){

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
    }

    void deallocate()
    


    };
}