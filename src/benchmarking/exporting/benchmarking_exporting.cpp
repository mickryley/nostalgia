#include "benchmarking_exporting.h"

#include "allocators/allocator_atlas.h"
#include "benchmarking/benchmark_atlas.h"
#include "implementations/implementation_atlas.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace nostalgia::benchmarking::exporting {

	namespace {
		std::vector<BenchmarkResult> m_currentBenchmarkResults;
	}

	void exportResultsToFile(const std::vector<BenchmarkResult>& results, const std::string& filename){
		json j;
		for (const auto& result : results) {
			j.push_back({
				{ "benchmarkLabel", benchmark::atlas.at(result.benchmarkID).label },
				{ "results", {
					// Implementation Details
					{"totalTime", result.totalTime},
					{"allocateTime", result.allocateTime},
					{"deallocateTime", result.deallocateTime},
					{"iterations", result.iterations},
					{"passes", result.passes},
					{"implementationParameters", result.implementation.parameters},

					// Implementation Type from Implementation Details ID
					{"implementationLabel", implementation::atlas.at(result.implementation.id).label},
					{"implementationDesc", implementation::atlas.at(result.implementation.id).desc},

					// Allocator Type from Implementation Details ID
					{"allocatorLabel", allocator::atlas.at(result.implementation.allocator).label},
					{"allocatorDescription", allocator::atlas.at(result.implementation.allocator).description},

					// Becnhmark Type from Benchmark ID
					//{"benchmarkLabel", benchmark::atlas.at(result.benchmarkID).label},
					{"benchmarkDesc", benchmark::atlas.at(result.benchmarkID).description},
				}}
			});
		}

		std::ofstream file(filename);
		if (file.is_open()) {
			file << j.dump(4); // Pretty print with 4 spaces
			file.close();
		}
		else {
			throw std::runtime_error("Could not open file for writing: " + filename);
		}
	}


	void addBenchmarkResult(const BenchmarkResult& result) {
		m_currentBenchmarkResults.push_back(result);
	}

	void exportBenchmark(BenchmarkResult results) {

	}

	void exportCurrentBenchmarks() {
		exportResultsToFile(m_currentBenchmarkResults, "benchmark_results.txt");
	}
}