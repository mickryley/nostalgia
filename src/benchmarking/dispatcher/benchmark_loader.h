#pragma once

#include "benchmarking/info/benchmark_meta.h"
#include "implementations/info/implementation_meta.h"

namespace nostalgia::benchmarking::loader {

    nostalgia::BenchmarkID get_benchmark_id();
    nostalgia::BenchmarkParams& get_parameters();
    nostalgia::AllocatorFlags get_allocator_flags();

	void add_allocator_to_benchmark(const nostalgia::AllocatorID id);
    void remove_allocator_from_benchmark(const nostalgia::AllocatorID id);
	bool is_allocator_in_benchmark(const nostalgia::AllocatorID id);

    void add_implementation_to_benchmark(const nostalgia::ImplementationID id);
    void remove_implementation_from_benchmark(const nostalgia::ImplementationID id);
    bool is_implementation_in_benchmark(const nostalgia::ImplementationID id);

    void load_benchmark(const nostalgia::BenchmarkID id);
    void dispatch_benchmark(const nostalgia::BenchmarkID id);
}