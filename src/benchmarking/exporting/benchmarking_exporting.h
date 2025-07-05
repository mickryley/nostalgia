#pragma once

#include "implementations/implementation_meta.h"
#include "benchmarking/benchmark_atlas.h"


#include <string>
#include <vector>

namespace nostalgia::benchmarking::exporting {

	struct BenchmarkResult {
		double totalTime;
		double allocateTime;
		double deallocateTime;
		int iterations;
		int passes;
		nostalgia::ImplementationDetails implementation;
		nostalgia::BenchmarkID benchmarkID;
	};

	void addBenchmarkResult(const BenchmarkResult& result);
	void exportBenchmark(BenchmarkResult results);
	void exportCurrentBenchmarks();
	void exportResultsToFile(const std::vector<BenchmarkResult>& results, const std::string& filename);
}