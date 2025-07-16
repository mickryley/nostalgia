//#include "benchmarking/dispatcher/macros.h" // Ensure is commented out at compile time
//#include "implementations/implementation_atlas.h"

nostalgia::BenchmarkID benchmark_id = nostalgia::BenchmarkID::IBM_Bursts;

template <typename object_type>
void run_static_contPointer(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index) {
	nostalgia::ImplementationID i_id = nostalgia::ImplementationID::ObjectOverride_StaticAccess_PointerContainer_ReverseDeallocation;

	// This must be templated to the specific object type used in the benchmark

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		auto** vec = new object_type * [iterations]; // [ALLOC SPECIFIC]

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = new object_type(CONSTRUCTOR_PARAMETERS);    // [ALLOC SPECIFIC]
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
template <typename object_type>
void run_static_contPointer_rewindDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index) {

	nostalgia::ImplementationID i_id = nostalgia::ImplementationID::ObjectOverride_StaticAccess_PointerContainer_RewindDeallocation;

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	//log::print("Checking Compatability of Rewind");
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)
	{
		START_ALLOC_TIMERS();

		auto** vec = new object_type * [iterations]; // [ALLOC SPECIFIC]

		for (size_t j = 0; j < iterations; j++) {
			vec[j] = new object_type(CONSTRUCTOR_PARAMETERS);    // [ALLOC SPECIFIC]
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		ALLOCATOR_GLOBAL_ACCESS.rewind();
		delete[] vec;

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}
template <typename object_type>
void run_static_contPointer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index) {

        nostalgia::ImplementationID i_id = nostalgia::ImplementationID::ObjectOverride_StaticAccess_PointerContainer_ForwardDeallocation; 

        IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
        CHECK_ALLOCATOR_COMPATABILITY();
        BEGIN_ALL_TIMERS();

        for (size_t i = 0; i < passes; i++)
        {
                START_ALLOC_TIMERS();

                auto** vec = new object_type * [iterations]; // [ALLOC SPECIFIC]

                for (size_t j = 0; j < iterations; j++) {
                        vec[j] = new object_type(CONSTRUCTOR_PARAMETERS);    // [ALLOC SPECIFIC]
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
template <typename object_type>
void run_malloc_contPointer_forwardDealloc(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index) {

	// {Implementation Specific}
	nostalgia::ImplementationID i_id = nostalgia::ImplementationID::NoAllocator_Malloc_PointerContainer_ForwardDeallocation; 

	IMPLEMENTATION_DETAILS(IBM_BURSTS_IMPLEMENTATION_DETAILS);
	CHECK_ALLOCATOR_COMPATABILITY();
	BEGIN_ALL_TIMERS();

	for (size_t i = 0; i < passes; i++)					// (Benchmarking) - 1 - Passes
	{
		START_ALLOC_TIMERS();

		auto** vec = new object_type * [iterations]; 	// [ALLOC SPECIFIC] {Implementation Specific}

		for (size_t j = 0; j < iterations; j++) {		// (Benchmarking) - 2 - Iterations - Allocations
			void* ptr = malloc(sizeof(object_type));  	// [ALLOC SPECIFIC] {Implementation Specific}
			if (!ptr) throw std::bad_alloc(); 			// {Implementation Specific}
			vec[j] = new (ptr) object_type(CONSTRUCTOR_PARAMETERS);  // {Implementation Specific}
		}

		PAUSE_ALLOC_TIMERS();
		START_DEALLOC_TIMERS();

		for (size_t k = 0; k < iterations; k++) {		// (Benchmarking) - 3 - Iterations - Deallocations
			vec[k]->~object_type(); 					// [ALLOC SPECIFIC] {Implementation Specific}
			std::free(vec[k]); 							// [ALLOC SPECIFIC] {Implementation Specific}
		}
		delete[] vec;									// [ALLOC SPECIFIC] {Implementation Specific}

		PAUSE_DEALLOC_TIMERS();
	}

	STOP_ALL_TIMERS();
	PRINT_ALL_TIMERS();
	EXPORT_BENCHMARK_RESULTS();
}

void run_ibmbursts_benchmark(nostalgia::AllocatorType allocator, size_t iterations, size_t passes, size_t object_id_index) {
	log::print("Running IBM Bursts benchmark with allocator: {} and parameters: iterations={}, passes={}, object_id_index={}",
		allocator.label, iterations, passes, object_id_index);

	// ObjectID index is used to select the object type for the benchmark
	switch(static_cast<nostalgia::ObjectID>(object_id_index)) {
		case nostalgia::ObjectID::Vector2D:
			run_static_contPointer<OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS_VECTOR2D>(allocator, iterations, passes, object_id_index);
			run_static_contPointer_rewindDealloc<OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS_VECTOR2D>(allocator, iterations, passes, object_id_index);
			run_static_contPointer_forwardDealloc<OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS_VECTOR2D>(allocator, iterations, passes, object_id_index);
			run_malloc_contPointer_forwardDealloc<OBJECT_BASIC_VECTOR2D>(allocator, iterations, passes, object_id_index);
			break;
		case nostalgia::ObjectID::Vector3D:
			run_static_contPointer<OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS_VECTOR3D>(allocator, iterations, passes, object_id_index);
			run_static_contPointer_rewindDealloc<OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS_VECTOR3D>(allocator, iterations, passes, object_id_index);
			run_static_contPointer_forwardDealloc<OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS_VECTOR3D>(allocator, iterations, passes, object_id_index);
			run_malloc_contPointer_forwardDealloc<OBJECT_BASIC_VECTOR3D>(allocator, iterations, passes, object_id_index);
			break;
		default:
			log::print(LogFlags::Error, "Invalid ObjectID index: {}", object_id_index);
			return;
	}
}

#include "allocators/defines/_clear_defines.inl"