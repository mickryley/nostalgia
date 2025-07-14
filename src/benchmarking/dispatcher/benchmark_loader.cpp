#include "benchmarking/dispatcher/benchmark_loader.h"
#include "benchmarking/info/benchmark_atlas.h"

#include "allocators/info/allocator_atlas.h"

#include "implementations/info/implementation_atlas.h"

#include "ui/gui/gui.h"

#include "utils/log.h"
#include <unordered_set>

namespace nostalgia::benchmarking::loader {

    namespace {
        nostalgia::BenchmarkParams loaded_benchmark_params;
        nostalgia::BenchmarkID loaded_benchmark_id = nostalgia::BenchmarkID::NONE;
		std::unordered_set<nostalgia::AllocatorID> selected_allocators;
        std::unordered_set<nostalgia::ImplementationID> selected_implementations;
    }

    nostalgia::BenchmarkID get_benchmark_id() {
        return loaded_benchmark_id;
    }

    nostalgia::AllocatorFlags get_allocator_flags() {
		return nostalgia::benchmark::atlas.at(loaded_benchmark_id).compatible_flags;
    }

    void add_allocator_to_benchmark(const nostalgia::AllocatorID id) {
        if (!selected_allocators.contains(id)) selected_allocators.insert(id);
    }

    void remove_allocator_from_benchmark(const nostalgia::AllocatorID id) {
        if (selected_allocators.contains(id)) selected_allocators.erase(id);
    }

    bool is_allocator_in_benchmark(const nostalgia::AllocatorID id) {
        return selected_allocators.contains(id);
    }

    void add_implementation_to_benchmark(const nostalgia::ImplementationID id) {
        if (!selected_implementations.contains(id)) selected_implementations.insert(id);
    }
    
    void remove_implementation_from_benchmark(const nostalgia::ImplementationID id) {
        if (selected_implementations.contains(id)) selected_implementations.erase(id);
    }

    bool is_implementation_in_benchmark(const nostalgia::ImplementationID id) {
        return selected_implementations.contains(id);
    }

    nostalgia::BenchmarkParams& get_parameters() {
        return loaded_benchmark_params;
    }

    void load_benchmark(const nostalgia::BenchmarkID id) {
        log::print("Loading benchmark...");

        loaded_benchmark_id = id;
		selected_allocators.clear();
        selected_implementations.clear();

        // Prefill with compatible allocators
        for (const auto& [aID, aType] : nostalgia::allocator::atlas) {
            if (aType.is_compatible_with(get_allocator_flags()))
				selected_allocators.insert(aID);
        }

        // Prefill with ALL implementations
        // Room to add filter for benchmark + selected allocators in future.
        for (const auto& [i_id, i_Type] : nostalgia::implementation::atlas) {
            //if (i_Type.is_compatible_with(get_allocator_flags()))
                selected_implementations.insert(i_id);
        }

        const auto& benchmark_type = nostalgia::benchmark::atlas.at(id);
        if (!benchmark_type.paramSpecs.empty()) {
            nostalgia::gui::load_benchmarking_params(benchmark_type.paramSpecs);
        } 
    }

      void dispatch_benchmark(const nostalgia::BenchmarkID id) {
        const auto& benchmark_type = nostalgia::benchmark::atlas.at(id);
        if (benchmark_type.dispatcher) {
            benchmark_type.dispatcher();
        }
    }
}