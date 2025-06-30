
#include <iostream>
#include <chrono>
#include <vector>
#include <memory>
#include <cstdlib>
#include <utility>
#include <memory_resource>
#include <array>



auto time_it = [](auto&& func, const std::string& label = "Duration") {
    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();

    std::forward<decltype(func)>(func)();  // Call the function/lambda

    auto end = clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << label << ": " << duration << " ms\n";
};

struct MyPoolAllocator {
    void* memory = nullptr;
    size_t capacity = 0;
    size_t size = 0;

    void init(size_t initial_capacity) {
        capacity = initial_capacity;
        size = 0;
        memory = std::malloc(capacity);
    }

    void* alloc(size_t bytes) {
        if (size + bytes > capacity) {
            size_t new_capacity = std::max(capacity * 2, size + bytes);
            void* new_memory = std::malloc(new_capacity);
            std::memcpy(new_memory, memory, size);
            std::free(memory);
            memory = new_memory;
            capacity = new_capacity;
        }
        void* ptr = static_cast<char*>(memory) + size;
        size += bytes;
        return ptr;
    }

    template<typename T, typename... Args>
    T* make(Args&&... args) {
        void* mem = alloc(sizeof(T));
        return new (mem) T(std::forward<Args>(args)...);
    }

    template<typename T>
    void destroy(T* ptr) {
        ptr->~T();
    }

    void reset() { size = 0; }

    ~MyPoolAllocator() {
        std::free(memory);
    }
};

struct MyOverridePoolAllocator {
    void* memory = nullptr;
    size_t capacity = 0;
    size_t size = 0;

    void init(size_t initial_capacity) {
        capacity = initial_capacity;
        size = 0;
        memory = std::malloc(capacity);
    }

    void* alloc(size_t bytes) {
        if (size + bytes > capacity) {
            size_t new_capacity = std::max(capacity * 2, size + bytes);
            void* new_memory = std::malloc(new_capacity);
            std::memcpy(new_memory, memory, size);
            std::free(memory);
            memory = new_memory;
            capacity = new_capacity;
        }

        void* ptr = static_cast<char*>(memory) + size;
        size += bytes;
        return ptr;
    }

    void free(void* ptr) {
        // No-op (bump allocator)
    }

    void reset() { size = 0; }

    ~MyOverridePoolAllocator() { std::free(memory); }
};

template<typename T>
struct MyStlAllocator {
    using value_type = T;

    MyStlAllocator() = default;

    template<typename U>
    constexpr MyStlAllocator(const MyStlAllocator<U>&) noexcept {}

    [[nodiscard]] T* allocate(std::size_t n) {
        return static_cast<T*>(std::malloc(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t) noexcept {
        std::free(p);
    }

    template<typename U>
    bool operator==(const MyStlAllocator<U>&) const noexcept { return true; }
    template<typename U>
    bool operator!=(const MyStlAllocator<U>&) const noexcept { return false; }
};

template<typename T>
struct MyStlConformedAllocator {
    using value_type = T;

    MyStlConformedAllocator(MyPoolAllocator& pool) : pool_(pool) {}
    template<typename U>
    MyStlConformedAllocator(const MyStlConformedAllocator<U>& other) noexcept : pool_(other.pool_) {}

    T* allocate(std::size_t n) {
        return static_cast<T*>(pool_.alloc(sizeof(T) * n));
    }

    void deallocate(T* p, std::size_t) noexcept {
        // no-op for pool
    }

    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) const {
        //static bool first = true;
        /*if (first) {
            std::cout << "[MyStlConformedAllocator::construct called ✅]\n";
            first = false;
        }*/
        new(p) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* p) const {
        p->~U();
    }

    template<typename U>
    bool operator==(const MyStlConformedAllocator<U>&) const noexcept { return true; }
    template<typename U>
    bool operator!=(const MyStlConformedAllocator<U>&) const noexcept { return false; }

    MyPoolAllocator& pool_;
};

template<typename T>
struct MyStlConformedWrappedPoolAllocator {
    using value_type = T;

    MyPoolAllocator* pool = nullptr;

    MyStlConformedWrappedPoolAllocator() = default;
    MyStlConformedWrappedPoolAllocator(MyPoolAllocator& p) : pool(&p) {}

    template<typename U>
    MyStlConformedWrappedPoolAllocator(const MyStlConformedWrappedPoolAllocator<U>& other) noexcept
        : pool(other.pool) {}

    T* allocate(std::size_t n) {
        return static_cast<T*>(pool->alloc(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t) noexcept {
        // No-op for bump allocators like yours
    }

    template<typename U>
    bool operator==(const MyStlConformedWrappedPoolAllocator<U>& other) const noexcept {
        return pool == other.pool;
    }

    template<typename U>
    bool operator!=(const MyStlConformedWrappedPoolAllocator<U>& other) const noexcept {
        return pool != other.pool;
    }
};

struct MyOverrideAllocatorSingleton {
    static MyOverridePoolAllocator& get() {
        static MyOverridePoolAllocator instance;
        return instance;
    }
};

struct MyObject {
    int x, y;
    MyObject(int a, int b) : x(a), y(b) {}
    ~MyObject() { volatile int sink = x + y; }
};

// Override requires a static allocator to be referred to 
struct MyObjectWithOverride {
    int x, y;

    static MyOverridePoolAllocator allocator;

    static void* operator new(std::size_t size) {
        return allocator.alloc(size);
    }

    static void operator delete(void* ptr) {
        allocator.free(ptr);
    }

    // Optional: placement-new support for internal moves/copies
    static void* operator new(std::size_t, void* ptr) noexcept { return ptr; }

    MyObjectWithOverride(int a, int b) : x(a), y(b) {}
    ~MyObjectWithOverride() { volatile int sink = x + y; }
};

struct MyOverrideObjectWithSingleton {
    int x, y;

    void* operator new(size_t size) {
        return MyOverrideAllocatorSingleton::get().alloc(size);
    }

    void operator delete(void* ptr) {
        MyOverrideAllocatorSingleton::get().free(ptr);
    }

    static void* operator new(size_t, void* ptr) noexcept { return ptr; }

    MyOverrideObjectWithSingleton(int a, int b) : x(a), y(b) {}
    ~MyOverrideObjectWithSingleton() { volatile int sink = x + y; }
};

// Allocate the static pool instance
MyOverridePoolAllocator MyObjectWithOverride::allocator;

void benchmark_template_alloc(const int count) {
    MyPoolAllocator alloc;
    alloc.init(sizeof(MyObject) * count);

    MyObject** objects = static_cast<MyObject**>(std::malloc(sizeof(MyObject*) * count));

    for (int i = 0; i < count; ++i) {
        objects[i] = alloc.make<MyObject>(i, i);
    }

    for (int i = 0; i < count; ++i) {
        alloc.destroy(objects[i]);
    }

    std::free(objects);
}

void benchmark_stl_conformed_wrapped_alloc(const int count) {
    //constexpr int count = 1000000;

    MyPoolAllocator pool;
    pool.init(sizeof(MyObject) * count);

    std::vector<MyObject, MyStlConformedWrappedPoolAllocator<MyObject>> objs{
        MyStlConformedWrappedPoolAllocator<MyObject>(pool)
    };
    

    for (int i = 0; i < count; ++i)
        objs.emplace_back(i, i);
    
    // Destructors will be called, memory won't be freed individually (by design)
}


void benchmark_traits_wrapped_vector(const int count) {
    MyPoolAllocator pool;
    pool.init(sizeof(MyObject) * count);

    using Alloc = MyStlConformedAllocator<MyObject>;
    using Traits = std::allocator_traits<Alloc>;

    Alloc alloc(pool);

    // Pre-allocate memory (uninitialized)
    MyObject* raw = Traits::allocate(alloc, count);

    // Wrap it in a vector using a custom deleter to skip double-deletion
    std::vector<MyObject, Alloc> objs(alloc);
    objs.reserve(count);

    for (int i = 0; i < count; ++i) {
        Traits::construct(alloc, raw + i, i, i);
        objs.emplace_back(std::move(raw[i]));
        Traits::destroy(alloc, raw + i);  // optional: clean up moved-from
    }

    Traits::deallocate(alloc, raw, count);
}
void benchmark_traits_vector_of_ptrs(const int count) {
    //constexpr int count = 1000000;

    MyPoolAllocator pool;
    pool.init(sizeof(MyObject) * count);

    using Alloc = MyStlConformedAllocator<MyObject>;
    using Traits = std::allocator_traits<Alloc>;

    Alloc alloc(pool);
    std::vector<MyObject*> ptrs;
    ptrs.reserve(count);

    for (int i = 0; i < count; ++i) {
        MyObject* p = Traits::allocate(alloc, 1);
        Traits::construct(alloc, p, i, i);
        ptrs.push_back(p);
    }

    for (auto* p : ptrs) {
        Traits::destroy(alloc, p);
        Traits::deallocate(alloc, p, 1);
    }
}

void benchmark_template_withVec_alloc(const int count) {
    //constexpr int count = 1000000;
    MyPoolAllocator alloc;
    alloc.init(sizeof(MyObject) * count);

    std::vector<MyObject*> objects;

    for (int i = 0; i < count; ++i) {
        objects.push_back(alloc.make<MyObject>(i, i));
    }

    for (int i = 0; i < count; ++i) {
        alloc.destroy(objects[i]);
    }

    // No Free required for the vector
}

void benchmark_operator_override_alloc(const int count) {
    //constexpr int count = 1000000;

    MyObjectWithOverride::allocator.init(sizeof(MyObjectWithOverride) * count);

    std::vector<MyObjectWithOverride*> objects;


    for (int i = 0; i < count; ++i) {
        objects.push_back(new MyObjectWithOverride(i, i));
    }

    for (auto* ptr : objects) {
        delete ptr;
    }

    // Pool is freed automatically when allocator goes out of scope
}

void benchmark_operator_override_singleton_alloc(const int count) {
    //constexpr int count = 1000000;

    MyOverrideAllocatorSingleton::get().init(sizeof(MyOverrideObjectWithSingleton) * count);

    std::vector<MyOverrideObjectWithSingleton*> objects;


    for (int i = 0; i < count; ++i) {
        objects.push_back(new MyOverrideObjectWithSingleton(i, i));
    }

    for (auto* ptr : objects) {
        delete ptr;
    }

    // Pool is freed automatically when allocator goes out of scope
}

void benchmark_minimal_alloc(const int count){
    std::vector<MyObject> objs;
    for (int i = 0; i < count; ++i) {
        objs.emplace_back(i, i);
    }
}

void benchmark_stl_allocator(const int count) {
    std::vector<MyObject, MyStlAllocator<MyObject>> objs;
    for (int i = 0; i < count; ++i) {
        objs.emplace_back(i, i);
    }
}



void benchmark_pmr_monotonic_alloc(const int count) {
    constexpr size_t buffer_size = sizeof(MyObject) * 10000000 + 1024;
    static std::array<std::byte, buffer_size> buffer;
    std::pmr::monotonic_buffer_resource resource(buffer.data(), buffer.size());
    std::pmr::vector<MyObject> objs(&resource);
    for (int i = 0; i < count; ++i) {
        objs.emplace_back(i, i);
    }
}

void benchmark_pmr_unsynchronized_pool_alloc(const int count) {
    //constexpr size_t buffer_size = sizeof(MyObject) * 100000 + 1024;
    //static std::array<std::byte, buffer_size> buffer;
    std::pmr::unsynchronized_pool_resource pool;
    std::pmr::vector<MyObject> objs(&pool);
    for (int i = 0; i < count; ++i) {
        objs.emplace_back(i, i);
    }
}

void benchmark_pmr_synchronized_pool_alloc(const int count) {
    //constexpr size_t buffer_size = sizeof(MyObject) * 100000 + 1024;
    //static std::array<std::byte, buffer_size> buffer;
    std::pmr::synchronized_pool_resource pool;
    std::pmr::vector<MyObject> objs(&pool);
    for (int i = 0; i < count; ++i) {
        objs.emplace_back(i, i);
    }
}

int legacy_main() {
#if defined(NDEBUG)
    std::cout << "Compiled in Release mode (NDEBUG defined)\n";
#else
    std::cout << "Compiled in Debug mode (NDEBUG NOT defined)\n";
#endif

    int totalAllocs = 10000000;
    using timer = std::chrono::high_resolution_clock;

    auto start = timer::now();
    benchmark_minimal_alloc(totalAllocs);
    auto end = timer::now();
    std::cout << "Total [Minimal] time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";


    start = timer::now();
    benchmark_template_alloc(totalAllocs);
    end = timer::now();
    std::cout << "Total [Template (with Object**)] time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";

    start = timer::now();
    benchmark_template_withVec_alloc(totalAllocs);
    end = timer::now();
    std::cout << "Total [Template (With Vector)] time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";


            
    start = timer::now();
    //benchmark_stl_conformed_wrapped_alloc(totalAllocs);
    end = timer::now();
    std::cout << "Total [Template (Stl Conform Wrapped)] time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";

    start = timer::now();
    benchmark_traits_wrapped_vector(totalAllocs);
    end = timer::now();
    std::cout << "Total [Template (Stl Conform Wrapped - Traits Wrapped Vector)] time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
              
    start = timer::now();
    benchmark_traits_vector_of_ptrs(totalAllocs);
    end = timer::now();
    std::cout << "Total [Template (Stl Conform Wrapped - Traits Wrapped Vector of Ptrs)] time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";
              
    start = timer::now();
    benchmark_operator_override_alloc(totalAllocs);
    end = timer::now();
    std::cout << "Total [Override (With Static)] time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";

    start = timer::now();
    benchmark_operator_override_singleton_alloc(totalAllocs);
    end = timer::now();
    std::cout << "Total [Override (With Singleton)] time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";

    start = timer::now();
    benchmark_stl_allocator(totalAllocs);
    end = timer::now();
    std::cout << "Total [STL (template)] time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";

    start = timer::now();
    benchmark_pmr_monotonic_alloc(totalAllocs);
    end = timer::now();
    std::cout << "Total [PMR (monotonic)] time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";


    start = timer::now();
    benchmark_pmr_unsynchronized_pool_alloc(totalAllocs);
    end = timer::now();
    std::cout << "Total [PMR (unsynch)] time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";

              
                  start = timer::now();
    benchmark_pmr_synchronized_pool_alloc(totalAllocs);
    end = timer::now();
    std::cout << "Total [PMR (synch)] time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms\n";

              

    return 0;
}
