#pragma once

#include <cstddef> 
#include <new>     
#include <memory>  
#include <cassert>
#include <memory_resource> 

namespace nostalgia::linear {
    
    // === Base Class ===
    class LinearAllocator {
    public: 
        LinearAllocator(std::byte* buf, size_t cap, const char* caller = "Unknown");
        void*       allocate(size_t bytes, size_t alignment = alignof(std::max_align_t));
        void        rewind();
    private:
        std::byte*  m_buffer;               
        size_t      m_capacity;             
        size_t      m_offset = 0;           

        size_t      m_peakCapacity = 0; 
    };
	
	// === Global Static Allocator ===
    extern LinearAllocator g_linear_allocator;

	// === Singleton Static Allocator ===
    struct SingletonLinearAllocator {
        static LinearAllocator& get_instance();
	};
        
    // === Template Allocator ===
    template <typename T>
    class LinearAllocatorTemplate {
    public:
        using value_type = T;
        LinearAllocatorTemplate() = default;
        template <typename U>
        LinearAllocatorTemplate(const LinearAllocatorTemplate<U>&) {}

        T* allocate(std::size_t n) {
            return static_cast<T*>(g_linear_allocator.allocate(n * sizeof(T), alignof(T)));
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
            (void)p; (void)n; // No-op: handled by allocator reset
        }

        void rewind() {
            g_linear_allocator.rewind();
		}
    };
// === Template Allocator - Singleton Access ===
    template <typename T>
    class LinearAllocatorTemplateSingletonAccess {
    public:
        using value_type = T;
        LinearAllocatorTemplateSingletonAccess() = default;
        template <typename U>
        LinearAllocatorTemplateSingletonAccess(const LinearAllocatorTemplateSingletonAccess<U>&) {}

        T* allocate(std::size_t n) {
            return static_cast<T*>(SingletonLinearAllocator::get_instance().allocate(n * sizeof(T), alignof(T)));
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
            (void)p; (void)n; // No-op: handled by allocator reset
        }

        void rewind() {
            SingletonLinearAllocator::get_instance().rewind();
		}
    };
    template <typename T>
    class LinearAllocatorStlTemplate {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;         // Old standard, now optional 
        using reference = T&;                   // Old standard, now optional 
        using const_reference = const T&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        template <typename U>
        struct rebind {
            using other = LinearAllocatorStlTemplate<U>;
        };

        LinearAllocatorStlTemplate() noexcept = default;
        template <typename U>
        LinearAllocatorStlTemplate(const LinearAllocatorStlTemplate<U>&) noexcept {}

        // STL Requried Allocate
        T* allocate(std::size_t n) {
            return static_cast<T*>(g_linear_allocator.allocate(n * sizeof(T), alignof(T)));
        }

        template<typename... Args>
        T* create(Args&&... args) {
            void* mem = allocate(1);
            return new (mem) T(std::forward<Args>(args)...);
        }

        // STL Requried Deallocate (no-op for linear)
        void deallocate(T* /*p*/, std::size_t /*n*/) noexcept {
            // No-op: handled by allocator reset
        }

        bool operator==(const LinearAllocatorStlTemplate&) const noexcept { return true; }
        bool operator!=(const LinearAllocatorStlTemplate&) const noexcept { return false; }

        void rewind() {
            g_linear_allocator.rewind();
        }
    };




    template<typename T>
    class LinearVector_TemplateAllocator {
    public:
        LinearVector_TemplateAllocator(size_t capacity, linear::LinearAllocatorTemplate<T>& allocator)
            : m_capacity(capacity), m_allocator(allocator)
        {
            m_data = m_allocator.allocate(capacity);
            m_size = 0;
        }

        LinearVector_TemplateAllocator(const LinearVector_TemplateAllocator&) = delete;
        LinearVector_TemplateAllocator& operator=(const LinearVector_TemplateAllocator&) = delete;

        template<typename... Args>
        T* emplace_back(Args&&... args) {
            assert(m_size < m_capacity && "Exceeded FastLinearVector capacity!");
            //T* obj = 
            new (&m_data[m_size]) T(std::forward<Args>(args)...);
            return &m_data[m_size++];
        }

        T& operator[](size_t index) { return m_data[index]; }
        const T& operator[](size_t index) const { return m_data[index]; }

        size_t size() const { return m_size; }
        size_t capacity() const { return m_capacity; }

        T* data() { return m_data; }
        const T* data() const { return m_data; }

        void clear() { m_size = 0; } // Does NOT call destructors

    private:
        T* m_data = nullptr;
        size_t m_size = 0;
        size_t m_capacity = 0;
        linear::LinearAllocatorTemplate<T>& m_allocator;
    };

    template<typename T>
    class LinearVector_StlTemplateAllocator {
    public:
        LinearVector_StlTemplateAllocator(size_t capacity, linear::LinearAllocatorStlTemplate<T>& allocator)
            : m_capacity(capacity), m_allocator(allocator)
        {
            m_data = m_allocator.allocate(capacity);
            m_size = 0;
        }

        // Not copyable
        LinearVector_StlTemplateAllocator(const LinearVector_StlTemplateAllocator&) = delete;
        LinearVector_StlTemplateAllocator& operator=(const LinearVector_StlTemplateAllocator&) = delete;

        // Push an object in-place
        template<typename... Args>
        T* emplace_back(Args&&... args) {
            assert(m_size < m_capacity && "Exceeded FastLinearVector capacity!");
            T* obj = new (&m_data[m_size]) T(std::forward<Args>(args)...);
            return &m_data[m_size++];
        }

        T& operator[](size_t index) { return m_data[index]; }
        const T& operator[](size_t index) const { return m_data[index]; }

        size_t size() const { return m_size; }
        size_t capacity() const { return m_capacity; }

        T* data() { return m_data; }
        const T* data() const { return m_data; }

        void clear() { m_size = 0; } // Does NOT call destructors

    private:
        T* m_data = nullptr;
        size_t m_size = 0;
        size_t m_capacity = 0;
        linear::LinearAllocatorStlTemplate<T>& m_allocator;
    };

    template<typename T>
    class PointerLinearVector_ObjectOverride {
    public:
        explicit PointerLinearVector_ObjectOverride(size_t capacity)
            : m_capacity(capacity)
        {
            m_data = new T * [capacity]; // Just a pointer array
            m_size = 0;
        }

        ~PointerLinearVector_ObjectOverride() {
            delete[] m_data; // Only deletes the pointer array � not the objects themselves
        }

        // Not copyable
        PointerLinearVector_ObjectOverride(const PointerLinearVector_ObjectOverride&) = delete;
        PointerLinearVector_ObjectOverride& operator=(const PointerLinearVector_ObjectOverride&) = delete;

        template<typename... Args>
        T* emplace_back(Args&&... args) {
            assert(m_size < m_capacity && "Exceeded PointerLinearVector_ObjectOverride capacity!");
            T* obj = new T(std::forward<Args>(args)...);  // Calls overridden operator new
            m_data[m_size++] = obj;
            return obj;
        }

        T* operator[](size_t index) { return m_data[index]; }
        const T* operator[](size_t index) const { return m_data[index]; }

        size_t size() const { return m_size; }
        size_t capacity() const { return m_capacity; }

        T** data() { return m_data; }
        const T* const* data() const { return m_data; }

        void clear() { m_size = 0; } // Doesn�t destroy objects (assumes allocator handles rewind)

    private:
        T** m_data = nullptr;
        size_t m_size = 0;
        size_t m_capacity = 0;
    };

    /*
        class LinearMemoryResource : public std::pmr::memory_resource {
        private:
            char* m_buffer;
            size_t m_offset = 0;
            size_t m_capacity;
        public:
            LinearMemoryResource(void* buf, std::size_t cap);
        protected:
            void* do_allocate(std::size_t bytes, std::size_t alignment) override;
            // No-op in linear
            void do_deallocate(void*, std::size_t, std::size_t) override;

            bool do_is_equal(const memory_resource& other) const noexcept override;
        };
    */


    // PMR Conformed Static Allocator
    // PMR Container would use std::pmr::polymorphic_allocator or similar
    // PMR Confirmed Singleton Allocator
	// PMR Container would use std::pmr::polymorphic_allocator or similar
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