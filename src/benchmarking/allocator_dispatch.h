#pragma once

#include "allocators/allocator_meta.h"

/*  This is the dispatch file for the IBM Bursts benchmark.
 
	There are the functions to be called from the UI or command line interface

	It filters allocators for compatible allocator types, runs their respective benchmarks.

	To keep things DRY while still being able to effectively benchmark templated, 
	polymorphic, and non-templated allocators, we instead avoid boilerplate code by
	using macros to define the benchmark function calls within the dispatch function
	from the allocator_dispatch.inc file.

	Therefore the only file that needs to be modified to add a new allocator
	across all benchmarks and implementations is the allocator_dispatch.inc file.
*/


namespace nostalgia::benchmarking::IBMBursts {

	void dispatch(); // Default to all allocators, empty for now

    void dispatch(AllocatorID allocator);
	void dispatch(AllocatorType allocator);
}