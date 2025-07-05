#include "ibmbursts.h" 

#include "timer.h"

// Rough temp exporting, change this later
#include "benchmarking/exporting/benchmarking_exporting.h"

// Difficulty here is we need to build this from an inc with only a define macro separating the differences
// They are namespace protected but we must remember to undef the macro after use to be clean.
// The macro must match the .inc used macro

// For now we can handwrite one implementation, and then move it to the inc file later.
// What we need to remember here is that we're defining multiple implementations of the same benchmarking function
// So all much be correct, and consistent across benchmarks.

// This is why the specifics of each implementation should be drawn from 
// ONE inc macro file that is consistent across all implementations.

// And equally the consistency of the benchmark function must be consistent across the allocators,
// Therefore details of the benchmark function should be drawn from another INC file that is consistent 
// within the benchmarking namespace.

#include "allocators/linear_bump/allocator_linear.h"
#include "allocators/stack_lifo/allocator_stack.h"
#include "allocators/pool_slab/allocator_pool.h"
#include "allocators/freelist_variable/allocator_freelist.h"

#include "objects/objects_linear.h"
#include "objects/objects_stack.h"
#include "objects/objects_pool.h"
#include "objects/objects_freelist.h"

#include "allocators/allocator_meta.h"
#include "benchmarking/benchmark_meta.h"

#include "implementations/implementation_meta.h"
#include "implementations/implementation_atlas.h"
#include "benchmarking/benchmark_atlas.h"

#include "benchmarking/macros.h"

struct ImplementationDetails {
	const char* allocator;
	const char* name;
	const char* desc;
};

namespace nostalgia::benchmarking::IBMBursts {
	//void run_ibmbursts_benchmark() {}

	namespace linear {
#include "allocators/linear_bump/defines_linear.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"

	}
	namespace stack {
#include "allocators/stack_lifo/defines_stack.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"

	}
	namespace pool {
#include "allocators/pool_slab/defines_pool.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"
	}
	namespace freelist {
#include "allocators/freelist_variable/defines_freelist.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"
	}
}