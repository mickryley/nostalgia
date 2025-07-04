#include "benchmarking/allocator_dispatch.h"

#include "benchmarking/benchmark_meta.h"
#include "benchmarking/benchmark_atlas.h"

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
        
		// Passes the allocator to ibmbursts.h for routed dispatching with selective implementation compatabilitiy
    #define RUN_BENCHMARK(ns) ns::run_ibmbursts_benchmark(allocator);
        
#include "benchmarking/allocator_dispatch.inc"        

    #undef RUN_BENCHMARK
    }
}