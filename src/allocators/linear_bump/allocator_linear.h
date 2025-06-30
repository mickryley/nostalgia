#pragma once

#include <cstddef> // For size_t, std::max_align_t
#include <new>     // For std::bad_alloc
#include <memory>  // For alignof

namespace nostalgia::linear {

    // No (Default) Implementation
    void linearAllocator();

    // Object Override Implementation (Static & Singleton Access)
    //void linearAllocatorObjectOverrideStatic();
    //void linearAllocatorObjectOverrideSingleton();

    class LinearAllocator {
    public: 
        LinearAllocator(char* buf, size_t cap);
        //virtual void* allocate(size_t) = 0;
        void* allocate(size_t bytes, size_t alignment = alignof(std::max_align_t));
        void rewind();
    private:
        char* m_buffer;      // Pointer to the start of the memory chunk
        size_t m_capacity;   // Total size of the memory chunk
        size_t m_offset;     // Current position for the next allocation

        size_t m_peakCapacity = 0; // Peak capacity used during allocations
    };

/*
    - Standard / None
- Global Override Static + Singleton
- Object Override Static + Singleton
    - Pointer Container Variant
    - STD Container Variant
    - Custom Container Variant
- Template Allocator
    - Pointer Container Variant
    - STD Container Variant
    - Custom Container Variant
- STL Conformed Template Allocator
    - Pointer Container Variant
    - STD Container Variant
- PMR Conformed Static Allocator
    - PMR Container
- PMR Confirmed Singleton Allocator
    - PMR Container

    */
}