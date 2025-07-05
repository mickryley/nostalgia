//#include "benchmarking/macros.h" // Ensure is commented out at compile time
//#include "implementations/implementation_atlas.h"

nostalgia::BenchmarkID benchmarkID = nostalgia::BenchmarkID::IBM_Bursts;

void run_static_v2d_contPointer(nostalgia::AllocatorType allocator, int iterations, int passes, int size) {

	nostalgia::ImplementationID iid = nostalgia::ImplementationID::OO_St_cP_RD;

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

void run_static_v2d_contPointer_rewindDealloc(nostalgia::AllocatorType allocator, int iterations, int passes, int size) {

	nostalgia::ImplementationID iid = nostalgia::ImplementationID::OO_St_cP_RwD;

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

void run_static_v2d_contPointer_forwardDealloc(nostalgia::AllocatorType allocator, int iterations, int passes, int size) {

	nostalgia::ImplementationID iid = nostalgia::ImplementationID::OO_St_cP_FD; 

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

void run_ibmbursts_benchmark(nostalgia::AllocatorType allocator, int iterations, int passes, int size) {

	run_static_v2d_contPointer(allocator, iterations, passes, size);
	run_static_v2d_contPointer_forwardDealloc(allocator, iterations, passes, size);
	run_static_v2d_contPointer_rewindDealloc(allocator, iterations, passes, size);
}

#undef OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS
#undef ALLOCATOR_STATIC_ACCESS 