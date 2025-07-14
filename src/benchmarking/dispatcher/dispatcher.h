#pragma once

/*  This is the dispatch header for all benchmarks.
 
	These are the entry points called by the Run / Dispatch button in the GUI or CLI.

	The dispatcher filters allocators for compatible allocator types, 
	and then runs their respective benchmarks in sequence.

	To keep things DRY whilst remaining able to effectively benchmark various methods of
	templating, polymorphic, and hard-coded non-templated allocators, we instead avoid 
	boilerplate code by using compiler macros.

	These macros are defined in the macros.h and per_allocator_dispatch.inc files.

	The sequence of operations is as follows:
	1. This dispatch() is called with a benchmark ID (Stored in the benchmark_loader).
	2. The dispatch() function is defined in a file unique to the benchmark (e.g. dispatch_ibmbursts.cpp), which are kept isolated by namespace.
	3. The dispatch() function performs compatability checks of all allocator types against the benchmark and follows the parameters passed to the loader.
	4. It then calls defines RUN_BENCHMARK as its own namespaced run_[benchmark_name]_benchmark() function, and includes the per_allocator_dispatch.inc file.
	5. The per_allocator_dispatch.inc file sequentially calls each allocator's correct run benchmark function, which is defined in a unique file for each BENCHMARK type. (eg. ibmbursts.h -> ibmbursts.cpp)
	6. These files construct the benchmark per allocator using .inl files unique to each allocator type (eg linear_defines.inl, stack_defines.inl, etc). and the actual benchmark logic (eg ibmbursts.inl).
	7. Therefore the entireity of the benchmark logic is defined in a single .inl file (eg ibmbursts.inl), which makes heavy use of the macros defined in macros.h to improve readability, reduce boilerplate, and highlight the differences between implementations.

	
	Therefore the only file that needs to be modified to add a new allocator logic across all benchmarks and implementations is the allocator_dispatch.inc file.
	However several routing files must also be updated:
	1. This [dispatcher.h] file must be updated to include the new namespaced dispatch() function.
	2. Define the new dispatch() in dispatch_[benchmark_name].cpp and correctly implement the RUN_BENCHMARK macro routing.
	3. Create a new [benchmark_name].h and [benchmark_name].cpp which pairs [allocator].inl with [benchmark_name].inl
*/

namespace nostalgia::benchmarking::IBMBursts {	void dispatch();	}
