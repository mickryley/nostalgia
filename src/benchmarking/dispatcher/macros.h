#pragma once

#pragma region General Implementation Macros

#define IMPLEMENTATION_DETAILS(_desc) \
    nostalgia::ImplementationDetails implementation_details = { \
        .id = i_id, \
        .allocator = allocator.id, \
        .parameters = _desc, \
		.run = nullptr \
    }; \

#define CHECK_ALLOCATOR_COMPATABILITY()      	if (!nostalgia::implementation::is_compatible_with_allocator(i_id, allocator)) { \
												log::print("Failed Implementation Compatability Check. [{} - {} - {}]", \
												nostalgia::allocator::get_label(implementation_details.allocator), \
												nostalgia::implementation::get_label(implementation_details.id), \
												implementation_details.parameters); \
												return; } 


#pragma endregion

#pragma region Timer Macros

#define BEGIN_ALL_TIMERS()			log::print("============================================"); \
									timer::Timer allocate_timer = timer::Timer(std::format("Allocate Timer {}", implementation_details.label())); \
									timer::Timer deallocate_timer = timer::Timer(std::format("Deallocate Timer {}", implementation_details.label())); \
									timer::Timer overall_timer = timer::Timer(std::format("Total Timer {}", implementation_details.label())); \
									overall_timer.start(); \
									log::print("============================================"); 

#define START_ALLOC_TIMERS()		allocate_timer.start();
#define PAUSE_ALLOC_TIMERS()		allocate_timer.pause();
#define START_DEALLOC_TIMERS()		deallocate_timer.start();
#define PAUSE_DEALLOC_TIMERS()		deallocate_timer.pause();

#define STOP_ALL_TIMERS()			overall_timer.stop();

#define PRINT_ALL_TIMERS()			log::print("============================================"); \
									allocate_timer.print(); \
									deallocate_timer.print(); \
									overall_timer.print(); \
									log::print("============================================"); 
#pragma endregion


#pragma region Export Results Macros

#define EXPORT_BENCHMARK_RESULTS() \
									exporting::BenchmarkResult result = exporting::BenchmarkResult{ \
										.total_time = overall_timer.print(), \
										.allocate_time = allocate_timer.print(), \
										.deallocate_time = deallocate_timer.print(), \
										.iterations = iterations, \
										.passes = passes, \
										.implementation = implementation_details, \
										.benchmark_id = benchmark_id, \
									}; \
									exporting::add_current_benchmark_result(result);
#pragma endregion
