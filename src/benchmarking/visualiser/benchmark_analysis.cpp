#include "benchmark_analysis.h"

#include <cstddef>
#include <array>
#include <vector>
#include <algorithm>

namespace nostalgia::visualiser {

    namespace {
        std::array<size_t, 3> fastest_local_indices = { 0, 0, 0 };
        std::array<size_t, 3> fastest_local_allocator_indices = { 0, 0, 0 };
        std::array<size_t, 3> fastest_local_implementation_indices = { 0, 0, 0 };

        size_t fastest_local_index = 0;
        size_t slowest_local_index = 0;
    }

	void reset_all_indices() {
		fastest_local_allocator_indices.fill(0);
		fastest_local_implementation_indices.fill(0);
        fastest_local_indices.fill(0);
        fastest_local_index = 0;
        slowest_local_index = 0;
	}

    void refresh_all_analysis(BenchmarkResults& results){
        reset_all_indices();

        find_fastest_local(results);
        find_fastest_local_allocator(results);
        find_fastest_local_implementation(results);
        fastest_local_index = fastest_local_indices[0];
    }


    size_t get_fastest_local_index() {
        if (fastest_local_index == SIZE_MAX) {
            return SIZE_MAX;
        }
        return fastest_local_index;
    }
    size_t get_slowest_local_index() {
        if (slowest_local_index == SIZE_MAX) {
            return SIZE_MAX;
        }
        return slowest_local_index;
    }

    std::array<size_t, 3>  get_fastest_local_indices(){
        return fastest_local_indices;
    }

    std::array<size_t, 3>  get_fastest_local_allocator_indices(){
        return fastest_local_allocator_indices;
    }

    std::array<size_t, 3>  get_fastest_local_implementation_indices(){
        return fastest_local_implementation_indices;
    }

    // Results are unsorted
    void find_slowest_local(BenchmarkResults& results) {
        if (results.local_results.empty()) {
            slowest_local_index = SIZE_MAX;
            return;
        }
        
        slowest_local_index = 0;
        for (size_t i = 1; i < results.local_results.size(); ++i) {
            if (results.local_results[i].total_time > results.local_results[slowest_local_index].total_time) {
                slowest_local_index = i;
            }
        }
    }

    void find_fastest_local(BenchmarkResults& results) {

        std::vector<size_t> valid_indices;
        for (size_t i = 0; i < results.local_results.size(); ++i) {
            if (results.local_results[i].total_time > 0) {
                valid_indices.push_back(i);
            }
        }

        while (valid_indices.size() < fastest_local_indices.size()) {
            valid_indices.push_back(SIZE_MAX);
        }

        std::partial_sort(
            valid_indices.begin(),
            valid_indices.begin() + fastest_local_indices.size(),
            valid_indices.end(),
            [&](size_t a, size_t b) {
                return results.local_results[a].total_time < results.local_results[b].total_time;
            }
        );

        for (size_t j = 0; j < fastest_local_indices.size(); ++j) {
            fastest_local_indices[j] = valid_indices[j];
        }
	}

    void find_fastest_local_allocator(BenchmarkResults& results) {
        // 1. Track the fastest index for each unique allocator
        std::unordered_map<AllocatorID, size_t> fastest_per_allocator;

        for (size_t i = 0; i < results.local_results.size(); ++i) {
            const auto& entry = results.local_results[i];
            if (entry.total_time <= 0)
                continue;

            AllocatorID id = entry.allocator_id;
            auto it = fastest_per_allocator.find(id);

            if (it == fastest_per_allocator.end() || 
                entry.total_time < results.local_results[it->second].total_time) 
            {
                fastest_per_allocator[id] = i;
            }
        }

        // 2. Gather indices and sort by time
        std::vector<size_t> indices;
        for (const auto& [_, idx] : fastest_per_allocator) {
            indices.push_back(idx);
        }

        // 3. Partial sort for top 3
        const size_t count = std::min(indices.size(), fastest_local_allocator_indices.size());
        std::partial_sort(
            indices.begin(), indices.begin() + static_cast<std::vector<size_t>::difference_type>(count), indices.end(),
            [&](size_t a, size_t b) {
                return results.local_results[a].total_time < results.local_results[b].total_time;
            }
        );

        // 4. Copy results, pad with SIZE_MAX
        for (size_t i = 0; i < fastest_local_allocator_indices.size(); ++i) {
            fastest_local_allocator_indices[i] = (i < count) ? indices[i] : SIZE_MAX;
        }
    }

    void find_fastest_local_implementation(BenchmarkResults& results) {
        // 1. Track the fastest index for each unique allocator
        std::unordered_map<ImplementationID, size_t> fastest_per_implementation;

        for (size_t i = 0; i < results.local_results.size(); ++i) {
            const auto& entry = results.local_results[i];
            if (entry.total_time <= 0)
                continue;

            ImplementationID id = entry.implementation_id;
            auto it = fastest_per_implementation.find(id);

            if (it == fastest_per_implementation.end() || 
                entry.total_time < results.local_results[it->second].total_time) 
            {
                fastest_per_implementation[id] = i;
            }
        }

        // 2. Gather indices and sort by time
        std::vector<size_t> indices;
        for (const auto& [_, idx] : fastest_per_implementation) {
            indices.push_back(idx);
        }

        // 3. Partial sort for top 3
        const size_t count = std::min(indices.size(), fastest_local_implementation_indices.size());
        std::partial_sort(
            indices.begin(), indices.begin() + static_cast<std::vector<size_t>::difference_type>(count), indices.end(),
            [&](size_t a, size_t b) {
                return results.local_results[a].total_time < results.local_results[b].total_time;
            }
        );

        // 4. Copy results, pad with SIZE_MAX
        for (size_t i = 0; i < fastest_local_implementation_indices.size(); ++i) {
            fastest_local_implementation_indices[i] = (i < count) ? indices[i] : SIZE_MAX;
        }
    }
	void analyse_results(BenchmarkResults& results) {
		if (results.local_results.empty()) {
			return;
		}

	}
}