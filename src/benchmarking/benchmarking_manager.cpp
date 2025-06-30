#include "benchmarking_manager.h"

#include "allocators/linear_bump/allocator_linear.h"

#include <iostream>

#include "log.h"

namespace nostalgia::benchmarking {
	void init_benchmarking_manager() {
		benchmark_linearAllocators();
	}

	void benchmark_linearAllocators_None() {
		std::cout << "Benchmarking linear allocators..." << std::endl;
		log::print("Benchmarking linear allocators...");
		//log::print(logFlags::WARN | logFlags::ERROR, "Benchmarking linear allocators...[{}][{}]", 1, 24);

		constexpr size_t bufferSize = 1024 * 1024; // 1 MB
		char* buffer = new char[bufferSize]; // Mallocate a buffer of 1 MB

		nostalgia::linear::LinearAllocator _allocator(buffer, bufferSize); // 1 MB

		void* a = _allocator.allocate(128);   // 128 bytes
		void* b = _allocator.allocate(256);   // 256 bytes

		_allocator.rewind();

	}

	void benchmark_linearAllocators() {
		benchmark_linearAllocators_None();
	}
}