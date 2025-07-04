//#include "benchmarking/macros.h" // Ensure is commented out at compile time

void run_static_v2d_contPointer(int iterations, int passes, int size) {

	IMPLEMENTATION_DETAILS("Static V2D Cont Pointer",
		std::format("{}x ({}x alloc + {}x free)", passes, iterations, iterations).c_str());
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

void run_static_v2d_contPointer_forwardDealloc(int iterations, int passes, int size) {

	IMPLEMENTATION_DETAILS("Static V2D Cont Pointer Forward Dealloc",
		std::format("{}x ({}x alloc + {}x free)", passes, iterations, iterations).c_str());
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

void run_ibmbursts_benchmark(int iterations, int passes, int size) {
	run_static_v2d_contPointer(iterations, passes, size);
	run_static_v2d_contPointer_forwardDealloc(iterations, passes, size);
}