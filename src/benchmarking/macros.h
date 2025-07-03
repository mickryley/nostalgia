#pragma once

#pragma region Implementation Macros
#define IMPLEMENTATION_DETAILS(_name, _desc) \
								ImplementationDetails _impDetails = { \
									NAME_ALLOCATOR, \
									_name, \
									_desc \
								}; \

#pragma endregion


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
