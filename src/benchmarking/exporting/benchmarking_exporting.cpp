#include "benchmarking_exporting.h"

#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

namespace nostalgia::benchmarking::exporting {

	void exportResultsToFile(const std::vector<BenchmarkResult>& results, const std::string& filename){
		json j;
		for (const auto& result : results) {
			j.push_back({
				{"totalTime", result.totalTime},
				{"allocateTime", result.allocateTime},
				{"deallocateTime", result.deallocateTime},
				{"iterations", result.iterations},
				{"label", result.label},
				{"allocator", result.allocator},
				{"description", result.description}
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
}