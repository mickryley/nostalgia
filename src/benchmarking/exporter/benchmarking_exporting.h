#pragma once

#include "implementations/info/implementation_meta.h"
#include "benchmarking/info/benchmark_atlas.h"


#include <string>
#include <vector>

namespace nostalgia::benchmarking::exporting {

	namespace {
		std::string local_source_label = "Local Results";
	}

	struct BenchmarkResult {
		double total_time;
		double allocate_time;
		double deallocate_time;
		size_t iterations;
		size_t passes;
		nostalgia::ImplementationDetails implementation;
		nostalgia::BenchmarkID benchmark_id;
	};

	std::string get_next_benchmark_filename(std::string& filename);
	void clear_current_benchmark_results();
	void add_current_benchmark_result(const BenchmarkResult& result);
	void exportBenchmark(BenchmarkResult results);
	std::string export_current_benchmarks();
	void exportResultsToFile(const std::vector<BenchmarkResult>& results, const std::string& filename, std::string& source_label = local_source_label);
}