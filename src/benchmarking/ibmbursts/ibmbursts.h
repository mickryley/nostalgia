#pragma once

#include "allocators/info/allocator_meta.h"

namespace nostalgia::benchmarking::IBMBursts {
	//void run_ibmbursts_benchmark(int argc, char** argv);
	// void run_ibmbursts_benchmark();

	// This is the main entry point for the benchmark after passing the compatability dispatch check.
	// At this point we need to apply the benchmark implementations to the different allocators
	// These are named identically, separated by namespaces to avoid conflicts

	// The actual logic is written in the matching .inl

	namespace linear {
		void run_ibmbursts_benchmark(nostalgia::AllocatorType allocator, size_t iterations = 1000, size_t passes = 5000);
	}
	namespace stack {
		void run_ibmbursts_benchmark(nostalgia::AllocatorType allocator, size_t iterations = 1000, size_t passes = 5000);
	}
	namespace pool{
		void run_ibmbursts_benchmark(nostalgia::AllocatorType allocator, size_t iterations = 1000, size_t passes = 5000);
	}
	namespace freelist{
		void run_ibmbursts_benchmark(nostalgia::AllocatorType allocator, size_t iterations = 1000, size_t passes = 5000);
	}
}