#pragma once

#include "allocators/info/allocator_meta.h"
#include "objects/info/object_meta.h"

namespace nostalgia::benchmarking::IBMBursts {
	//void run_ibmbursts_benchmark(int argc, char** argv);
	// void run_ibmbursts_benchmark();

	// This is the main entry point for the benchmark after passing the compatability dispatch check.
	// At this point we need to apply the benchmark implementations to the different allocators
	// These are named identically, separated by namespaces to avoid conflicts

	// The actual logic is written in the matching .inl

	namespace {
		size_t iterations_default = 1000;
		size_t passes_default = 5000;
		size_t object_id_index_default = static_cast<size_t>(nostalgia::ObjectID::Vector2D);
	}

	#define CALL_RUN_IBMBURSTS_BENCHMARK void run_ibmbursts_benchmark(nostalgia::AllocatorType allocator, size_t iterations = iterations_default, size_t passes = passes_default, size_t object_id_index = object_id_index_default) 

	namespace default_malloc { CALL_RUN_IBMBURSTS_BENCHMARK; }
	namespace linear { CALL_RUN_IBMBURSTS_BENCHMARK; }
	namespace stack { CALL_RUN_IBMBURSTS_BENCHMARK; }
	namespace pool{ CALL_RUN_IBMBURSTS_BENCHMARK; }
	namespace freelist{ CALL_RUN_IBMBURSTS_BENCHMARK; }

	#undef CALL_RUN_IBMBURSTS_BENCHMARK
}