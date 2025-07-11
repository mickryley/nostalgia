#pragma once

#pragma region General Implementation Macros

#define IMPLEMENTATION_DETAILS(_desc) \
    nostalgia::ImplementationDetails _impDetails = { \
        .id = iid, \
        .allocator = allocator.id, \
        .parameters = _desc, \
		.run = nullptr \
    }; \

#define CHECK_ALLOCATOR_COMPATABILITY()      	if (!nostalgia::implementation::isCompatibleWithAllocator(iid, allocator)) { \
												log::print("Failed Implementation Compatability Check. [{} - {} - {}]", \
												nostalgia::allocator::getLabel(_impDetails.allocator), \
												nostalgia::implementation::getLabel(_impDetails.id), \
												_impDetails.parameters); \
												return; } 


#pragma endregion

#pragma region Implementation Specific Macros



#pragma endregion

#pragma region Timer Macros

#define BEGIN_ALL_TIMERS()			log::print("============================================"); \
									timer::Timer _allocateTimer = timer::Timer(std::format("Allocate Timer {}", _impDetails.label())); \
									timer::Timer _deallocateTimer = timer::Timer(std::format("Deallocate Timer {}", _impDetails.label())); \
									timer::Timer _timer = timer::Timer(std::format("Total Timer {}", _impDetails.label())); \
									_timer.start(); \
									log::print("============================================"); 

#define START_ALLOC_TIMERS()		_allocateTimer.start();
#define PAUSE_ALLOC_TIMERS()		_allocateTimer.pause();
#define START_DEALLOC_TIMERS()		_deallocateTimer.start();
#define PAUSE_DEALLOC_TIMERS()		_deallocateTimer.pause();

#define STOP_ALL_TIMERS()			_timer.stop();

#define PRINT_ALL_TIMERS()			log::print("============================================"); \
									_allocateTimer.print(); \
									_deallocateTimer.print(); \
									_timer.print(); \
									log::print("============================================"); 
#pragma endregion


#pragma region Export Results Macros

#define EXPORT_BENCHMARK_RESULTS() \
									exporting::BenchmarkResult result = exporting::BenchmarkResult{ \
										.totalTime = _timer.print(), \
										.allocateTime = _allocateTimer.print(), \
										.deallocateTime = _deallocateTimer.print(), \
										.iterations = iterations, \
										.passes = passes, \
										.implementation = _impDetails, \
										.benchmarkID = benchmarkID, \
									}; \
									exporting::addBenchmarkResult(result);
#pragma endregion
