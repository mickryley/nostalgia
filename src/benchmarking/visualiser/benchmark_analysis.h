#pragma once

#include "benchmark_visualiser.h"

#include <array>

namespace nostalgia::visualiser {

    std::array<size_t, 3>  get_fastest_local_indices();
    std::array<size_t, 3>  get_fastest_local_allocator_indices();
    std::array<size_t, 3>  get_fastest_local_implementation_indices();

    void find_fastest_local(BenchmarkResults& results);
	void find_fastest_local_allocator(BenchmarkResults& results);
    void find_fastest_local_implementation(BenchmarkResults& results);
    
}