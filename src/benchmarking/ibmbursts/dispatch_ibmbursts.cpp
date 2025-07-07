#include "benchmarking/allocator_dispatch.h"

#include "benchmarking/benchmark_meta.h"
#include "benchmarking/benchmark_atlas.h"
#include "benchmarking/benchmark_loader.h"

#include "allocators/allocator_meta.h"
#include "allocators/allocator_atlas.h"

#include "implementations/implementation_atlas.h"
#include "ibmbursts.h"

#include "log.h"

namespace nostalgia::benchmarking::IBMBursts {

    namespace {
        constexpr BenchmarkID m_benchmarkID = nostalgia::BenchmarkID::IBM_Bursts;
    }

    void dispatch() {
        nostalgia::log::print(nostalgia::logFlags::DEBUG, "Dispatching IBM Bursts benchmark for all compatible allocators.");

        for (const auto& [allocatorID, allocator] : nostalgia::allocator::atlas) {
            if (allocator.isCompatibleWith(nostalgia::benchmark::getCompatibleFlags(m_benchmarkID))) {
                dispatch(allocator);
            } else {
                log::print(logFlags::WARN, "Allocator type [{}] is not compatible with IBM Bursts benchmark.", allocator.label);
            }
        }
    }

    void dispatch(nostalgia::AllocatorID allocatorID) {
        dispatch(nostalgia::allocator::atlas.at(allocatorID));
    }

    void dispatch(nostalgia::AllocatorType allocator){
		log::print("\n==========================================================\n");
        nostalgia::log::print(nostalgia::logFlags::DEBUG, "Dispatching IBM Bursts benchmark for allocator type [{}].", allocator.label);

		nostalgia::AllocatorFlags m_compatibleFlags = nostalgia::benchmark::getCompatibleFlags(m_benchmarkID);

        if (!allocator.isCompatibleWith(m_compatibleFlags)) {
            log::print(logFlags::WARN, "Allocator type [{}] is not compatible with IBM Bursts benchmark.", allocator.label);
            return;
        }
        
        // Parse the parameters from the command line or GUI
        // Stored in the loader

        nostalgia::BenchmarkParams& params = nostalgia::benchmarking::loader::getParameters();

        log::print("Before editing, passes = {}", params.get<int>("passes"));
        
        if (!params.contains("passes")) {
            log::print(logFlags::DEBUG, "No 'passes' parameter found, setting default to 5000.");
            params.set("passes", 5000); // Default to 5000 passes
        }
        if (!params.contains("iterations")) {
            log::print(logFlags::DEBUG, "No 'iterations' parameter found, setting default to 1000.");
            params.set("iterations", 1000); // Default to 1000 iterations
        }
        if (!params.contains("object")) {
            log::print(logFlags::DEBUG, "No 'object' parameter found, setting default to false.");
            params.set("object", 0); // Default to false (no object override)
        }
		// Passes the allocator to ibmbursts.h for routed dispatching with selective implementation compatabilitiy

    #define RUN_BENCHMARK(ns) ns::run_ibmbursts_benchmark(allocator, params.get<int>("iterations"), params.get<int>("passes"));
        
#include "benchmarking/allocator_dispatch.inc"        

    #undef RUN_BENCHMARK
    }
}