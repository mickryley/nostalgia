#pragma once

#include <vector>
#include <unordered_map>
#include <string>

namespace nostalgia::visualiser {

	
	struct BenchmarkPlotData {
		// Can change this to use the ID lookup but just using the string for now
		std::string benchmark_label;
		std::string benchmark_description; 
		std::string allocator_label;
		std::string allocator_description;
		std::string implementation_label;
		std::string implementation_description;
		std::string implementation_parameters;
		double total_time;
		double allocate_time;
		double deallocate_time;
		size_t iterations;
		size_t passes;
	};

	namespace {
		std::vector<BenchmarkPlotData> current_benchmark_plot_data;
		std::unordered_map<std::string, std::vector<BenchmarkPlotData>> current_benchmark_plot_data_map;
		int current_hovered_index;
		BenchmarkPlotData* current_hovered_data = nullptr;
	}

	// Key is a Benchmark Label, Value are all results matching that label
	std::unordered_map<std::string, std::vector<BenchmarkPlotData>> load_results_from_file(const std::string& path);

	// Only blindly loads all results - need to filter by allocator and test type later
	std::vector<BenchmarkPlotData> load_benchmark_plot_data(const std::string& path);

	void load_benchmark_plot_data();

	void draw_benchmark_hover_details();

	void draw_benchmark_plot();
	void draw_benchmark_plot(const std::vector<BenchmarkPlotData>& data);
}