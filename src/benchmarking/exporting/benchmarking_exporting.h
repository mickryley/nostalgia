#pragma once

#include "implementations/implementation_meta.h"


#include <string>
#include <vector>

namespace nostalgia::benchmarking::exporting {

	struct BenchmarkResult {
		double totalTime;
		double allocateTime;
		double deallocateTime;
		int iterations;
		nostalgia::ImplementationDetails implementation;
		//std::string allocator;
		//std::string label;
		//std::string description;
	};


	void exportBenchmark(BenchmarkResult results);
	// Function to export benchmark results to a file

	void exportResultsToFile(const std::vector<BenchmarkResult>& results, const std::string& filename);
}