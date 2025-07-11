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
        nostalgia::log::print(nostalgia::logFlags::DEBUG, "Filtering IBM Bursts benchmark for all selected allocators in loader.");

        for (const auto& [allocatorID, allocator] : nostalgia::allocator::atlas) {

            if (nostalgia::benchmarking::loader::isAllocatorInBenchmark(allocatorID)) {
                if (allocator.isCompatibleWith(nostalgia::benchmark::getCompatibleFlags(m_benchmarkID))) {
                    dispatch(allocator);
                }
                else {
                    log::print(logFlags::WARN, "Allocator type [{}] is not compatible with IBM Bursts benchmark.", allocator.label);
                }
            }
            else {
                log::print(logFlags::INFO, "Allocator type [{}] is not selected by the loader for this benchmark.", allocator.label);
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
        
        nostalgia::BenchmarkParams* params = &nostalgia::benchmarking::loader::getParameters();
        
		// Passes the allocator to ibmbursts.h for routed dispatching with selective implementation compatabilitiy

    #define RUN_BENCHMARK(ns) ns::run_ibmbursts_benchmark(allocator, static_cast<size_t>(params->get<int>("iterations").value_or(1000)), static_cast<size_t>(params->get<int>("passes").value_or(5000)));
        
#include "benchmarking/allocator_dispatch.inc"        

    #undef RUN_BENCHMARK
    }
}