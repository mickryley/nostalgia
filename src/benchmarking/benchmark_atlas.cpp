#include "benchmark_atlas.h"

#include "allocator_dispatch.h"

namespace nostalgia::benchmark {
	std::unordered_map<nostalgia::BenchmarkID, nostalgia::BenchmarkType> atlas{
		{ { nostalgia::BenchmarkID::IBM_Bursts, nostalgia::BenchmarkType{
			"IBM Bursts",
			"IBM Bursts Of Alloc and Dealloc",
			AllocatorFlags::NONE,
			false,
			[]() { nostalgia::benchmarking::IBMBursts::dispatch(); }
			} }
		}
	};

}