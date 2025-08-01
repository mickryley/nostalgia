#include "benchmarking/dispatcher/dispatcher.h"

#include "benchmarking/info/benchmark_meta.h"
#include "benchmarking/info/benchmark_atlas.h"
#include "benchmarking/dispatcher/benchmark_loader.h"

#include "allocators/info/allocator_meta.h"
#include "allocators/info/allocator_atlas.h"

#include "implementations/info/implementation_atlas.h"
#include "ibmbursts_entry_point.h"


#include "utils/log.h"

namespace nostalgia::benchmarking::IBMBursts {

    namespace {
        constexpr BenchmarkID m_benchmark_id = nostalgia::BenchmarkID::IBM_Bursts;

        // Temporarily hardcoding limits, actual maximum is 1MB / sizeof(Object)
        // Currently IBM takes V2D or V3D so 8 bytes or 12 bytes 
        // However the Pool implementation is fixed size at 16 bytes so ~65'000 iterations
        // And the Freelist has headers and footers of 8 bytes each so ~ 32'000 iterations max
        int max_iterations = 30000;
        int max_passes = 100000;
    }

    void sanitise_parameters(){
        nostalgia::BenchmarkParams* params = &nostalgia::benchmarking::loader::get_parameters();

        // Clamp Parameters 
        if (!params->contains("iterations")) {
            log::print(LogFlags::Warn, "No iterations parameter found. Setting to default value of 1000.");
            params->set<int>("iterations", 1000);
        } else if (params->get<int>("iterations").value() <= 0) {
            log::print(LogFlags::Error, "Invalid iterations value: {}. Must be greater than 0.", params->get<int>("iterations").value());
            return;
        } else if (params->get<int>("iterations").value() > max_iterations) {
            log::print(LogFlags::Warn, "Iterations value {} is too high. Setting to {}.", params->get<int>("iterations").value(), max_iterations);
            params->set<int>("iterations", max_iterations);
        }
        if (!params->contains("passes")) {
            log::print(LogFlags::Warn, "No passes parameter found. Setting to default value of 5000.");
            params->set<int>("passes", 5000);
        } else if (params->get<int>("passes").value() <= 0) {
            log::print(LogFlags::Error, "Invalid passes value: {}. Must be greater than 0.", params->get<int>("passes").value());
            return;
        } else if (params->get<int>("passes").value() > max_passes) {
            log::print(LogFlags::Warn, "Passes value {} is too high. Setting to {}.", params->get<int>("passes").value(), max_passes);
            params->set<int>("passes", max_passes);
        }
    }

    void dispatch(nostalgia::AllocatorType allocator){
		log::print("\n==========================================================\n");
        nostalgia::log::print(nostalgia::LogFlags::Debug, "Dispatching IBM Bursts benchmark for allocator type [{}].", allocator.label);

        if (!allocator.is_compatible_with_benchmark(nostalgia::benchmark::get_required_flags(m_benchmark_id))) {
            log::print(LogFlags::Warn, "Allocator type [{}] is not compatible with IBM Bursts benchmark.", allocator.label);
            return;
        }

        sanitise_parameters();
        nostalgia::BenchmarkParams* params = &nostalgia::benchmarking::loader::get_parameters();
        
		// Passes the allocator to ibmbursts.h for routed dispatching with selective implementation compatabilitiy
    #define RUN_BENCHMARK(ns) ns::run_ibmbursts_benchmark(allocator, static_cast<size_t>(params->get<int>("iterations").value_or(1000)), static_cast<size_t>(params->get<int>("passes").value_or(5000)), static_cast<size_t>(params->get<int>("object").value_or(static_cast<int>(nostalgia::ObjectID::Vector2D))));
        
#include "benchmarking/dispatcher/per_allocator_dispatch.inc"        

    #undef RUN_BENCHMARK
    }

    void dispatch(nostalgia::AllocatorID allocator_id) {
        dispatch(nostalgia::allocator::atlas.at(allocator_id));
    }

    void dispatch() {
        nostalgia::log::print(nostalgia::LogFlags::Debug, "Dispatching IBM Bursts benchmark for all compatible allocators.");
        nostalgia::log::print(nostalgia::LogFlags::Debug, "Filtering IBM Bursts benchmark for all selected allocators in loader.");

        for (const auto& [allocatorID, allocator] : nostalgia::allocator::atlas) {

            // Filter for selected allocators
            if (nostalgia::benchmarking::loader::is_allocator_in_benchmark(allocatorID)) {
                if (allocator.is_compatible_with_benchmark(nostalgia::benchmark::get_required_flags(m_benchmark_id))) {
                    dispatch(allocator);
                }
                else {
                    log::print(LogFlags::Warn, "Allocator type [{}] is not compatible with IBM Bursts benchmark.", allocator.label);
                }
            }
            else {
                log::print(LogFlags::Info, "Allocator type [{}] is not selected by the loader for this benchmark.", allocator.label);
            }
        }
    }
}