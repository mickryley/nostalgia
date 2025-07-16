#include "ibmbursts.h" 

#include "utils/timer.h"

// Rough temp exporting, change this later
#include "benchmarking/exporter/benchmarking_exporting.h"

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

#include "allocators/concrete/default_malloc/allocator_default.h"
#include "allocators/concrete/linear_bump/allocator_linear.h"
#include "allocators/concrete/stack_lifo/allocator_stack.h"
#include "allocators/concrete/pool_slab/allocator_pool.h"
#include "allocators/concrete/freelist_variable/allocator_freelist.h"

#include "objects/objects_linear.h"
#include "objects/objects_stack.h"
#include "objects/objects_pool.h"
#include "objects/objects_freelist.h"
#include "objects/objects.h"

#include "allocators/info/allocator_meta.h"
#include "benchmarking/info/benchmark_meta.h"

#include "implementations/info/implementation_meta.h"
#include "implementations/info/implementation_atlas.h"
#include "benchmarking/info/benchmark_atlas.h"

#include "benchmarking/dispatcher/benchmark_loader.h"
#include "benchmarking/dispatcher/macros.h"

struct ImplementationDetails {
	const char* allocator;
	const char* name;
	const char* desc;
};

namespace nostalgia::benchmarking::IBMBursts {

	namespace default_malloc{
#include "allocators/defines/malloc_defines.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"
	}
	namespace linear {
#include "allocators/defines/linear_defines.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"
	}
	namespace stack {
#include "allocators/defines/stack_defines.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"
	}
	namespace pool {
#include "allocators/defines/pool_defines.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"
	}
	namespace freelist {
#include "allocators/defines/freelist_defines.inl"
#include "benchmarking/ibmbursts/ibmbursts.inl"
	}
}