#pragma once

#include <cstddef> // For size_t, std::max_align_t
#include <new>     // For std::bad_alloc
#include <memory>  // For alignof
#include <cassert>
#include <memory_resource> // For pmr

namespace nostalgia::stack::scoped {

    namespace {
        constexpr size_t bufferSize = 1024 * 1024; // 1 MB
        char* buffer = new char[bufferSize]; // Mallocate a buffer of 1 MB
    }

    struct StackBlockHeader {
        uint32_t size; // True size / offset including the header
    };

    class StackAllocator {
    public:
        StackAllocator(char* buf, size_t cap);
        void* allocate(size_t bytes/*, size_t alignment = alignof(std::max_align_t)*/);
        bool free(void* ptr) noexcept;
        void reset() noexcept;
        void rewind() noexcept;
    private:
        char* m_buffer;            // Pointer to the start of the memory chunk
        size_t m_capacity;         // Total size of the memory chunk
        size_t m_offset = 0;           // Current position for the next allocation

        // Logging tools - remove on minimal / benchmarked version
        size_t m_peakCapacity = 0; // Peak capacity used during allocations 

        size_t calcAlignSize(size_t size, size_t alignment) const {
            return (size + alignment - 1) / alignment * alignment;
        }
    };

    static StackAllocator s_stackAllocator{ buffer, bufferSize };
}