/*#include <vector>
#include <random>

#include "objects/objects.h"

// 🟩 Fixed-size alloc/free pattern
template<typename Allocator>
void run_fixed_alloc_test(Allocator& alloc){
    std::vector<Obj_Vector2*> objs;
    constexpr size_t count = 10'000;

    for (int i = 0; i < count; ++i) {
        objs.push_back(alloc_construct<Obj_Vector2>(alloc, i, i));
    }
    for (auto* ptr : objs) {
        destruct_free(alloc, ptr);
    }
}

// 🟨 Variable-size, partial lifetime churn
template<typename Allocator>
void run_variable_alloc_test(Allocator& alloc){
        std::vector<void*> active;
        std::mt19937 rng(12345);
        std::uniform_int_distribution<size_t> sizeDist(8, 512);
        std::bernoulli_distribution retainDist(0.7);

        constexpr size_t Steps = 5000;
        for (size_t i = 0; i < Steps; ++i) {
            size_t size = sizeDist(rng);
            void* mem = alloc.alloc(size);
            if (retainDist(rng)) {
                active.push_back(mem);
            } else {
                alloc.free(mem);
            }
        }

        for (void* mem : active) {
            alloc.free(mem);
        }
    }

    // 🟥 Fragmentation stress: alternating alloc/free
    template<typename Allocator>
    void run_fragmentation_test(Allocator& alloc) {
        constexpr size_t Count = 4096;
        std::vector<void*> blocks(Count);

        // Step 1: Allocate all
        for (size_t i = 0; i < Count; ++i) {
            blocks[i] = alloc.alloc(128);
        }

        // Step 2: Free every other block
        for (size_t i = 0; i < Count; i += 2) {
            alloc.free(blocks[i]);
            blocks[i] = nullptr;
        }

        // Step 3: Attempt to re-allocate larger blocks
        std::vector<void*> largeBlocks;
        for (size_t i = 0; i < Count / 2; ++i) {
            void* b = alloc.alloc(256);
            if (b) largeBlocks.push_back(b);
        }

        // Cleanup
        for (void* ptr : blocks) {
            if (ptr) alloc.free(ptr);
        }
        for (void* ptr : largeBlocks) {
            alloc.free(ptr);
        }
    }

*/