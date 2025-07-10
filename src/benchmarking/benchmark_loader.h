#pragma once

#include "benchmarking/benchmark_meta.h"
#include "implementations/implementation_meta.h"

namespace nostalgia::benchmarking::loader {

    nostalgia::BenchmarkID getBenchmarkID();
    nostalgia::BenchmarkParams& getParameters();
    nostalgia::AllocatorFlags getAllocatorFlags();

	void addAllocatorToBenchmark(const nostalgia::AllocatorID id, bool add = true);
	bool isAllocatorInBenchmark(const nostalgia::AllocatorID id);

    void addImplementationToBenchmark(const nostalgia::ImplementationID id, bool add = true);
    bool isImplementationInBenchmark(const nostalgia::ImplementationID id);

    void loadBenchmark(const nostalgia::BenchmarkID id);

    void dispatchBenchmark(const nostalgia::BenchmarkID id);
}