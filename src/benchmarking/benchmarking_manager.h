#pragma once

#include "benchmark_meta.h"

namespace nostalgia::benchmarking {
	void init_benchmarking_manager();

	std::vector<BenchmarkType> getAllBenchmarks();
}