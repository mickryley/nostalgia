#pragma once

namespace BENCHMARKING{
    template<typename Allocator>
    void run_fixed_alloc_test(Allocator& alloc);

    template<typename Allocator>
    void run_variable_alloc_test(Allocator& alloc);

    template<typename Allocator>
    void run_fragmentation_test(Allocator& alloc);

    #include "benchmarking.inl"
}