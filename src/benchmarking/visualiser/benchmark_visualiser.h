#pragma once

#include <vector>
#include <unordered_map>
#include <string>

namespace nostalgia::visualiser {

	
	struct BenchmarkPlotData {
		// Can change this to use the ID lookup but just using the string for now
		std::string benchmarkLabel;
		std::string benchmarkDesc; 
		std::string allocatorLabel;
		std::string allocatorDescription;
		std::string implementationLabel;
		std::string implementationDesc;
		std::string implementationParameters;
		float totalTime;
		float allocateTime;
		float deallocateTime;
		int iterations;
		int passes;
	};

	namespace {
		std::vector<BenchmarkPlotData> m_currentPlotData;
		std::unordered_map<std::string, std::vector<BenchmarkPlotData>> m_benchmarkPlotDataMap;
		int m_hoveredIndex;
		BenchmarkPlotData* m_hoveredData = nullptr;
	}

	// Key is a Benchmark Label, Value are all results matching that label
	std::unordered_map<std::string, std::vector<BenchmarkPlotData>> loadResultsFromFile(const std::string& path);

	// Only blindly loads all results - need to filter by allocator and test type later
	std::vector<BenchmarkPlotData> loadBenchmarkPlotData(const std::string& path);

	void loadBenchmarkPlotData();

	void ShowDetails();

	void ShowBenchmarkPlot();
	void ShowBenchmarkPlot(const std::vector<BenchmarkPlotData>& data);
}