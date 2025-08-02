#pragma once

#include <cstddef> 
#include <new>     
#include <memory>  
#include <cassert>
#include <memory_resource>

namespace nostalgia::stack {

	// === Base Class ===
    class StackAllocator {
    public:
        StackAllocator(std::byte* buf, size_t cap, const char* caller = "Unknown");
        void*       allocate(size_t bytes);
        bool        free(void* ptr) noexcept;
        void        reset() noexcept;
        void        rewind() noexcept;
    private:
        std::byte*  m_buffer;            // Pointer to the start of the memory chunk
        size_t      m_capacity;              // Total size of the memory chunk
        size_t      m_offset = 0;            // Current position for the next allocation

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

    // === Template Allocator ===
    template <typename T>
    class StackAllocatorTemplate {
    public:
        using value_type = T;       
        StackAllocatorTemplate() = default;
        template <typename U>
        StackAllocatorTemplate(const StackAllocatorTemplate<U>&) {}
        T* allocate(std::size_t n) {
            return static_cast<T*>(g_stack_allocator.allocate(n * sizeof(T)));
        }
        template<typename... Args>
        T* create(Args&&... args) {
            void* mem = allocate(1); // one T-sized block
            return new (mem) T(std::forward<Args>(args)...);

        }
        void destroy(T* p) {
            if (p) p->~T(); // Explicitly call the destructor
        }
        void deallocate(T* p, std::size_t n) noexcept {
            (void)n; 
            g_stack_allocator.free(reinterpret_cast<std::byte*>(p));
        }
        void rewind() {
            g_stack_allocator.rewind();
        }
    };

    // === Template Allocator ===
    template <typename T>
    class StackAllocatorTemplateSingletonAccess {
    public:
        using value_type = T;       
        StackAllocatorTemplateSingletonAccess() = default;
        template <typename U>
        StackAllocatorTemplateSingletonAccess(const StackAllocatorTemplateSingletonAccess<U>&) {}
        T* allocate(std::size_t n) {
            return static_cast<T*>(SingletonStackAllocator::get_instance().allocate(n * sizeof(T)));
        }
        template<typename... Args>
        T* create(Args&&... args) {
            void* mem = allocate(1); // one T-sized block
            return new (mem) T(std::forward<Args>(args)...);

        }
        void destroy(T* p) {
            if (p) p->~T(); // Explicitly call the destructor
        }
        void deallocate(T* p, std::size_t n) noexcept {
            (void)n; 
            SingletonStackAllocator::get_instance().free(reinterpret_cast<std::byte*>(p));
        }
        void rewind() {
            SingletonStackAllocator::get_instance().rewind();
        }
    };
}