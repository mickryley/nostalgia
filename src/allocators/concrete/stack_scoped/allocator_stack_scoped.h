#pragma once

#include <cstddef>
#include <new>    
#include <memory> 
#include <cassert>
#include <memory_resource>

namespace nostalgia::stack::scoped {

    // === Base Class ===
    class StackAllocator {
    public:
        StackAllocator(std::byte* buf, size_t cap, const char* caller = "Unknown");
        void*       allocate(size_t bytes/*, size_t alignment = alignof(std::max_align_t)*/);
        bool        free(void* ptr) noexcept;
        void        reset() noexcept;
        void        rewind() noexcept;
    private:
        std::byte*  m_buffer;            // Pointer to the start of the memory chunk
        size_t      m_capacity;         // Total size of the memory chunk
        size_t      m_offset = 0;           // Current position for the next allocation

        size_t      m_peakCapacity = 0; // Peak capacity used during allocations 

        size_t      calculate_aligned_size(size_t size, size_t alignment) const {
            return (size + alignment - 1) / alignment * alignment;
        }
    };

    // === Global Static Allocator ===
    extern StackAllocator g_stack_allocator;

    // === Singleton Static Allocator ===
    struct SingletonStackAllocator {
        static StackAllocator& get_instance();
    };
}