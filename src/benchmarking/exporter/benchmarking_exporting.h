#pragma once

#include "implementations/info/implementation_meta.h"
#include "benchmarking/info/benchmark_atlas.h"


#include <string>
#include <vector>

namespace nostalgia::benchmarking::exporting {

	struct BenchmarkResult {
		double total_time;
		double allocate_time;
		double deallocate_time;
		size_t iterations;
		size_t passes;
		nostalgia::ImplementationDetails implementation;
		nostalgia::BenchmarkID benchmark_id;
	};

	void add_benchmark_result(const BenchmarkResult& result);
	void exportBenchmark(BenchmarkResult results);
	void export_current_benchmarks();
	void exportResultsToFile(const std::vector<BenchmarkResult>& results, const std::string& filename);
}