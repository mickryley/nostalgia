#include "benchmark_analysis.h"

#include <cstddef>
#include <array>
#include <vector>
#include <algorithm>

namespace nostalgia::visualiser {
	namespace {
		std::array<size_t, 3> fastest_local_allocator_indices = { 0, 0, 0 };
		std::array<size_t, 3> fastest_local_implementation_indices = { 0, 0, 0 };
	}

	void reset_all_indices() {
		fastest_local_allocator_indices.fill(0);
		fastest_local_implementation_indices.fill(0);
	}

	void find_fastest_local_allocator(BenchmarkResults& results) {
        std::vector<size_t> valid_indices;
        for (size_t i = 0; i < results.local_results.size(); ++i) {
            if (results.local_results[i].total_time > 0) {
                valid_indices.push_back(i);
            }
        }

        while (valid_indices.size() < fastest_local_allocator_indices.size()) {
            valid_indices.push_back(SIZE_MAX);
        }

        std::partial_sort(
            valid_indices.begin(),
            valid_indices.begin() + fastest_local_allocator_indices.size(),
            valid_indices.end(),
            [&](size_t a, size_t b) {
                return results.local_results[a].total_time < results.local_results[b].total_time;
            }
        );

        for (size_t j = 0; j < fastest_local_allocator_indices.size(); ++j) {
            fastest_local_allocator_indices[j] = valid_indices[j];
        }
	}

	void analyse_results(BenchmarkResults& results) {
		if (results.local_results.empty()) {
			return;
		}

	}
}