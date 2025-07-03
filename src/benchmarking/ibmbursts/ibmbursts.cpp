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


#include "objects/objects_pool.h"
#include "objects/objects_freelist.h"

#include "benchmarking/macros.h"

struct ImplementationDetails {
	const char* allocator;
	const char* name;
	const char* desc;
};


namespace nostalgia::benchmarking::IBMBursts {
	//void run_ibmbursts_benchmark(int argc, char** argv);

	/*
#pragma region Timer Macros
#define BEGIN_ALL_TIMERS()			timer::Timer _allocateTimer = timer::Timer(std::format("Allocate Timer {} {}", _impDetails.name, _impDetails.allocator)); \
									timer::Timer _deallocateTimer = timer::Timer("Deallocate Timer "); \
									timer::Timer _timer = timer::Timer("Total Timer "); \
									_timer.start();

#define START_ALLOC_TIMERS()		_allocateTimer.start();
#define PAUSE_ALLOC_TIMERS()		_allocateTimer.pause();
#define START_DEALLOC_TIMERS()		_deallocateTimer.start();
#define PAUSE_DEALLOC_TIMERS()		_deallocateTimer.pause();

#define STOP_ALL_TIMERS()			_timer.stop();

#define PRINT_ALL_TIMERS()			_allocateTimer.print(); \
									_deallocateTimer.print(); \
									_timer.print();
#pragma endregion

#pragma region Export Results Macros
#define EXPORT_BENCHMARK_RESULTS() \
								exporting::BenchmarkResult result = exporting::BenchmarkResult{ \
									.totalTime = _timer.print(), \
									.allocateTime = _allocateTimer.print(), \
									.deallocateTime = _deallocateTimer.print(), \
									.iterations = iterations, \
									.allocator = _impDetails.allocator, \
									.label = _impDetails.name, \
									.description = _impDetails.desc \
								}; \
								std::vector<exporting::BenchmarkResult> results = { result }; \
								exporting::exportResultsToFile(results, "benchmark_results.txt");
#pragma endregion
*/

#pragma region Allocation Specific Macros - These must be set for every namespace 
//#define OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS	nostalgia::freelist::objects::Vector2D_LocalOverride_StaticAccess
#pragma endregion
	void run_ibmbursts_benchmark() {}


	// Need to use namespaces to designate the functions to be used in the benchmark with a shared name
	// We need to piece this together from two separate lists of macros
	// But first we'll write it out by hand
	// Some definitions need to be specific to the allocator type

	namespace linear {
		void run_ibmbursts_benchmark(int iterations, int passes, int size){}
	}
	namespace stack {
		void run_ibmbursts_benchmark(int iterations, int passes, int size){}
	}
	namespace pool {
#pragma region Allocation Specific Macros - These must be set for every namespace 
#define NAME_ALLOCATOR		"Pool Allocator"
#define OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS	nostalgia::pool::objects::Vector2D_LocalOverride_StaticAccess
#pragma endregion

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
#undef OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS
	}
	namespace freelist {
#pragma region Allocation Specific Macros - These must be set for every namespace 
#define NAME_ALLOCATOR		"Freelist Allocator"
#define OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS	nostalgia::freelist::objects::Vector2D_LocalOverride_StaticAccess
#pragma endregion

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

#undef OBJECT_LOCAL_OVERRIDE_STATIC_ACCESS
	}
}