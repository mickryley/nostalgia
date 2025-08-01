#include "benchmarking_exporting.h"

#include "allocators/info/allocator_atlas.h"
#include "benchmarking/info/benchmark_atlas.h"
#include "implementations/info/implementation_atlas.h"

#include <fstream>
#include <format>

#include "utils/json_wrapped.h"

using json = nlohmann::json;

namespace nostalgia::benchmarking::exporting {

	namespace {

		std::string file_extension = ".json";
		std::string current_benchmark_filename = "current_benchmark_results";

		std::vector<BenchmarkResult> current_benchmark_results;

	}

	void export_results_to_file(const std::vector<BenchmarkResult>& results, const std::string& filename, std::string& source_label){
		json j;
		for (const auto& result : results) {
			j.push_back({
				{ "benchmark_id", result.benchmark_id },
				{ "benchmark_label", benchmark::atlas.at(result.benchmark_id).label },
				{ "results_source", source_label },
				{"compiler", get_compiler_name()},
				{"compiler_info", get_compiler_info()},
				{ "results", {
					// Implementation Details
					{"total_time", result.total_time},
					{"allocate_time", result.allocate_time},
					{"deallocate_time", result.deallocate_time},
					{"iterations", result.iterations},
					{"passes", result.passes},
					{"implementation_parameters", result.implementation.parameters},

					// Implementation Type from Implementation Details ID
					{"implementation_id", result.implementation.id},
					{"implementation_label", implementation::atlas.at(result.implementation.id).label},
					{"implementation_description", implementation::atlas.at(result.implementation.id).desc},

					// Allocator Type from Implementation Details ID
					{"allocator_id", allocator::atlas.at(result.implementation.allocator).id},
					{"allocator_description", allocator::atlas.at(result.implementation.allocator).description},
					{"allocator_label", allocator::atlas.at(result.implementation.allocator).label},
					{"allocator_description", allocator::atlas.at(result.implementation.allocator).description},

					// Becnhmark Type from Benchmark ID
					//{"benchmark_label", benchmark::atlas.at(result.benchmark_id).label},

					{"benchmark_description", benchmark::atlas.at(result.benchmark_id).description},

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



	void clear_current_benchmark_results() {
		current_benchmark_results.clear();
	}

	void add_current_benchmark_result(const BenchmarkResult& result) {
		current_benchmark_results.push_back(result);
	}

	std::string export_current_benchmarks() {
		std::string export_filename = get_next_benchmark_filename(current_benchmark_filename);
		export_results_to_file(current_benchmark_results, export_filename);
		return export_filename;
	}

	std::string get_next_benchmark_filename(std::string& filename) {
		int index = 0;
		std::string full_filename;
		do {
			full_filename = std::format("{}_{}{}", filename, index, file_extension);
			index++;
		} while	 (std::filesystem::exists(full_filename));
		return full_filename;
	}

	std::string get_compiler_name() {
#if defined(__clang__)
		return "Clang " __clang_version__;
#elif defined(__GNUC__)
		return "GCC " __VERSION__;
#elif defined(_MSC_VER)
		return "MSVC " + std::to_string(_MSC_VER);
#else
		return "Unknown Compiler";
#endif
	}

	std::string get_compiler_info() {
#if defined(COMPILER_INFO)
		return std::format("Compiler: {}", COMPILER_INFO);
#else
	return "Compiler info not available.";
#endif
	}

}