#include "benchmarking/dispatcher/dispatcher.h"

#include "benchmarking/info/benchmark_meta.h"
#include "benchmarking/info/benchmark_atlas.h"
#include "benchmarking/dispatcher/benchmark_loader.h"

#include "allocators/info/allocator_meta.h"
#include "allocators/info/allocator_atlas.h"

#include "implementations/info/implementation_atlas.h"
#include "ibmbursts.h"


#include "utils/log.h"

namespace nostalgia::benchmarking::IBMBursts {

    namespace {
        constexpr BenchmarkID m_benchmark_id = nostalgia::BenchmarkID::IBM_Bursts;
    }

    void dispatch(nostalgia::AllocatorType allocator){
		log::print("\n==========================================================\n");
        nostalgia::log::print(nostalgia::LogFlags::Debug, "Dispatching IBM Bursts benchmark for allocator type [{}].", allocator.label);

        if (!allocator.is_compatible_with_benchmark(nostalgia::benchmark::get_required_flags(m_benchmark_id))) {
            log::print(LogFlags::Warn, "Allocator type [{}] is not compatible with IBM Bursts benchmark.", allocator.label);
            return;
        }
        
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