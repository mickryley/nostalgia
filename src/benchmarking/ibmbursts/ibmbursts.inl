//#include "benchmarking/dispatcher/macros.h" // Ensure is commented out at compile time
//#include "implementations/implementation_atlas.h"

nostalgia::BenchmarkID benchmark_id = nostalgia::BenchmarkID::IBM_Bursts;

void run_static_v2d_contPointer(nostalgia::AllocatorType allocator, size_t iterations, size_t passes) {
	nostalgia::ImplementationID i_id = nostalgia::ImplementationID::ObjectOverride_StaticAccess_PointerContainer_ReverseDeallocation;

	// Parameterise the object being passed.
	// set the locally used object

	IMPLEMENTATION_DETAILS(std::format("{}x ({}x alloc + {}x free)", passes, iterations, iterations).c_str());
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		auto** vec = new OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS * [iterations]; // [ALLOC SPECIFIC]

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = new OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS(1.0f, 2.0f);    // [ALLOC SPECIFIC]
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (int k = static_cast<int>(iterations) - 1; k >= 0; --k) {
			delete vec[k];
		}

		delete[] vec;

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}

void run_static_v2d_contPointer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes) {

	nostalgia::ImplementationID i_id = nostalgia::ImplementationID::ObjectOverride_StaticAccess_PointerContainer_RewindDeallocation;

	IMPLEMENTATION_DETAILS(std::format("{}x ({}x alloc + {}x free)", passes, iterations, iterations).c_str());
	//log::print("Checking Compatability of Rewind");
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		auto** vec = new OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS * [iterations]; // [ALLOC SPECIFIC]

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = new OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS(1.0f, 2.0f);    // [ALLOC SPECIFIC]
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		ALLOCATOR_STATIC_ACCESS.rewind();
		delete[] vec;

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}

void run_static_v2d_contPointer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes) {

        nostalgia::ImplementationID i_id = nostalgia::ImplementationID::ObjectOverride_StaticAccess_PointerContainer_ForwardDeallocation; 

        IMPLEMENTATION_DETAILS(std::format("{}x ({}x alloc + {}x free)", passes, iterations, iterations).c_str());
        CHECK_ALLOCATOR_COMPATABILITY();
        BEGIN_ALL_TIMERS();

        for (size_t i = 0; i < passes; i++)
        {
                START_ALLOC_TIMERS();

                auto** vec = new OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS * [iterations]; // [ALLOC SPECIFIC]

                for (size_t j = 0; j < iterations; j++) {
                        vec[j] = new OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS(1.0f, 2.0f);    // [ALLOC SPECIFIC]
                }

                PAUSE_ALLOC_TIMERS();
                START_DEALLOC_TIMERS();

                for (size_t k = 0; k < iterations; k++) {
                        delete vec[k];
                }

                delete[] vec;

                PAUSE_DEALLOC_TIMERS();
        }

        STOP_ALL_TIMERS();
        PRINT_ALL_TIMERS();
        EXPORT_BENCHMARK_RESULTS();
}

void run_malloc_v2d_contPointer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes) {

	nostalgia::ImplementationID i_id = nostalgia::ImplementationID::NoAllocator_Malloc_PointerContainer_ForwardDeallocation; 

	IMPLEMENTATION_DETAILS(std::format("{}x ({}x alloc + {}x free)", passes, iterations, iterations).c_str());
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		auto** vec = new OBJECT_BASIC_VECTOR2D * [iterations]; // [ALLOC SPECIFIC]

		for (size_t j = 0; j < iterations; j++) {
			void* ptr = malloc(sizeof(OBJECT_BASIC_VECTOR2D));  // [ALLOC SPECIFIC]
			if (!ptr) throw std::bad_alloc(); // Safety check
			vec[j] = new (ptr) OBJECT_BASIC_VECTOR2D(1.0f, 2.0f);
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {
			vec[k]->~Vector2D(); // Call destructor
			std::free(vec[k]); // Free memory
		}

		delete[] vec;

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}

void run_ibmbursts_benchmark(nostalgia::AllocatorType allocator, size_t iterations, size_t passes) {
	log::print("Running IBM Bursts benchmark with allocator: {} and parameters: iterations={}, passes={}",
		allocator.label, iterations, passes);
	run_static_v2d_contPointer(allocator, iterations, passes);
	run_static_v2d_contPointer_forwardDealloc(allocator, iterations, passes);
	run_static_v2d_contPointer_rewindDealloc(allocator, iterations, passes);
	run_malloc_v2d_contPointer_forwardDealloc(allocator, iterations, passes);
}

#include "allocators/defines/_clear_defines.inl"