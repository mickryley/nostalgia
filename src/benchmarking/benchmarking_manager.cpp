#include "benchmarking_manager.h"

#include "benchmarking_ibmbursts/benchmarking_ibmbursts.h"

#include "benchmarking/allocator_dispatch.h"

#include "allocators/linear_bump/allocator_linear.h"
#include "allocators/stack_lifo/allocator_stack.h"
#include "allocators/pool_slab/allocator_pool.h"

#include "objects/objects_linear.h"
#include "objects/objects.h"

#include "allocators/allocator_meta.h"
#include "benchmarking/benchmark_meta.h"
#include "implementations/implementation_meta.h"


#include "timer.h"
#include "log.h"

#include <array>
#include <memory_resource>


#include "benchmarking/benchmark_atlas.h"
#include "allocators/allocator_atlas.h"
#include "implementations/implementation_atlas.h"

namespace nostalgia::benchmarking {


	enum class allocatorImplementation {
		ObjectOverrideStatic,
		ObjectOverrideSingleton
	};

	void benchmark_linearAllocators_None() {
		std::cout << "Benchmarking linear allocators..." << std::endl;
		log::print("Benchmarking linear allocators...");
		//log::print(logFlags::WARN | logFlags::ERROR, "Benchmarking linear allocators...[{}][{}]", 1, 24);

		constexpr size_t bufferSize = 1024 * 1024; // 1 MB
		char* buffer = new char[bufferSize]; // Mallocate a buffer of 1 MB

		nostalgia::stack::StackAllocator _allocator(buffer, bufferSize); // 1 MB

		void* a = _allocator.allocate(128);   // 128 bytes
		void* b = _allocator.allocate(256);   // 256 bytes
		void* c = _allocator.allocate(512);   // 256 bytes

		_allocator.free(c);
		_allocator.free(b);
		_allocator.free(a);

		_allocator.rewind();

		void* d = _allocator.allocate(512);   // 256 bytes

		_allocator.rewind();

		void* e = _allocator.allocate(513);   // 256 bytes

		_allocator.rewind();

		void* f = _allocator.allocate(bufferSize - 1);   // 256 bytes

		_allocator.rewind();

		void* g = _allocator.allocate(bufferSize);   // 256 bytes

		_allocator.rewind();

		void* h = _allocator.allocate(bufferSize + 1);   // 256 bytes

	}

	void init_benchmarking_manager() {
		//IBMBursts::benchmark_IBMBursts_linearAllocators();
		//IBMBursts::benchmark_IBMBursts_stackAllocators();
		//IBMBursts::benchmark_IBMBursts_poolAllocators();
		IBMBursts::benchmark_IBMBursts_freelistAllocators();

		//benchmark_linearAllocators_None();
	}
	// Temp hardcoding, load in DD later
	/*
	std::vector<AllocatorType> getAllAllocators(){
		return {
			{
				.id = AllocatorID::Linear,
				.label = "Linear Allocator",
				.description = "Linear Allocator",
				.requiredFlags = AllocatorFlags::NONE
			},
			{
				.id = AllocatorID::Stack,
				.label = "Stack Allocator",
				.description = "Stack Allocator",
				.requiredFlags = AllocatorFlags::NONE
			},
			{
				.id = AllocatorID::Pool,
				.label = "Pool Allocator",
				.description = "Pool Allocator",
				.requiredFlags = AllocatorFlags::FIXED_SIZE
			},
			{
				.id = AllocatorID::Freelist,
				.label = "Freelist Allocator",
				.description = "Freelist Allocator",
				.requiredFlags = AllocatorFlags::NONE
			},
		};
	}
	*/


	// Read this in from another data block or file in the future
	// Keep naming consistent so only the namespace needs changing
	std::vector<BenchmarkType> getAllBenchmarks() {
		/*std::vector<BenchmarkType> _all = {};

		for (const auto& bench : nostalgia::benchmark::atlas) {

			for (const auto& imp : nostalgia::implementation::atlas) {
				_all.push_back{
					.label = bench
				}
			}
		}*/
		return {
			{
				.label = "IBM Bursts Linear",
				.description = "IBM Bursts",
				//.compatibleFlags = AllocatorFlags::FIXED_SIZE,
				.dispatcher = {[]() { IBMBursts::dispatch(AllocatorID::Linear); }}
			},
			{
				.label = "IBM Bursts Stack",
				.description = "IBM Bursts",
				//.compatibleFlags = AllocatorFlags::FIXED_SIZE,
				//.disabled = true,
				.dispatcher = {[]() { IBMBursts::dispatch(AllocatorID::Stack); }}
			},
			{
				.label = "IBM Bursts Pool",
				.description = "IBM Bursts",
				//.compatibleFlags = AllocatorFlags::FIXED_SIZE,
				//.disabled = true,
				.dispatcher = {[]() { IBMBursts::dispatch(AllocatorID::Pool); }}
			},
			{
				.label = "IBM Bursts Freelist",
				.description = "IBM Bursts",
				//.compatibleFlags = AllocatorFlags::FIXED_SIZE,
				//.disabled = true,
				.dispatcher = {[]() { IBMBursts::dispatch(AllocatorID::Pool); }}
			}
		};
	}

}