#include "benchmarking/allocator_dispatch.h"

#include "allocators/allocator_meta.h"
#include "ibmbursts.h"

#include "log.h"

namespace nostalgia::benchmarking::IBMBursts {

    namespace {
        AllocatorFlags m_compatibleFlags = AllocatorFlags::FIXED_SIZE;
    }

    void dispatch() {
        log::print(logFlags::DEBUG, "Dispatching IBM Bursts benchmark for all compatible allocators.");
    }
    void dispatch(AllocatorType allocator){
		log::print(logFlags::DEBUG, "Dispatching IBM Bursts benchmark for allocator type [{}].", allocator.label);

		// Compatability check for the allocator type
        if (!allocator.isCompatibleWith(m_compatibleFlags)) {
            log::print(logFlags::WARN, "Allocator type [{}] is not compatible with IBM Bursts benchmark.", allocator.label);
            return;
        }
        
    // The full range of benchmark implementations is run from within the allocator namespace
    #define RUN_BENCHMARK(ns) ns::run_ibmbursts_benchmark();
        
#include "benchmarking/allocator_dispatch.inc"        

    #undef RUN_BENCHMARK
    }
}