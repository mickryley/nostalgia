#pragma once

#include "benchmark_visualiser.h"

#include <array>

namespace nostalgia::visualiser {

    size_t get_fastest_local_index();
    size_t get_slowest_local_index();
    std::array<size_t, 3>  get_fastest_local_indices();
    std::array<size_t, 3>  get_fastest_local_allocator_indices();
    std::array<size_t, 3>  get_fastest_local_implementation_indices();

    void refresh_all_analysis(BenchmarkResults& results);

    void find_slowest_local(BenchmarkResults& results);
    void find_fastest_local(BenchmarkResults& results);
	void find_fastest_local_allocator(BenchmarkResults& results);
    void find_fastest_local_implementation(BenchmarkResults& results);
    
}