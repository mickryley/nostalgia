#pragma once

#include <vector>
#include <unordered_map>
#include <string>

namespace nostalgia::visualiser {

    enum class ImportMapKey {
        NONE,
        BenchmarkLabel,
        ResultsSource
    };

	struct BenchmarkPlotData {
		// Can change this to use the ID lookup but just using the string for now
		std::string results_source;
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

	struct BenchmarkResults {
		std::string benchmark_label;
		std::vector<BenchmarkPlotData> local_results; 
		std::unordered_map<std::string, std::vector<BenchmarkPlotData>> reference_results; // Key is a source, Value are ALL matching results
	};

	// Key is a Benchmark Label, Value are ALL matching results
	std::unordered_map<std::string, std::vector<BenchmarkPlotData>> load_results_from_file(
		const std::string& path, 
		ImportMapKey map_key, 
		ImportMapKey import_filter_for = ImportMapKey::NONE,
		std::string benchmark_filter = "");

	// void load_benchmark_plot_data();
	void load_reference_benchmark_results(std::string benchmark_label);
	void load_local_benchmark_results(std::string path);
	void draw_benchmark_hover_details();

	void draw_benchmark_results_view(size_t index);
	void draw_benchmark_plot();
	void draw_benchmark_plot(const std::vector<BenchmarkPlotData>& data);

	size_t get_total_displayed_benchmark_results();
	std::string get_benchmark_label(size_t index);
}