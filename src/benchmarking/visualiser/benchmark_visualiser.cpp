#include "benchmark_visualiser.h"

//#include <nlohmann/json.hpp>
#include "utils/json_wrapped.h"
#include <fstream>

#include <imgui.h>

#include "utils/implot_wrapped.h"

#include "utils/log.h"
#include "utils/return_codes.h"

using json = nlohmann::json;

namespace nostalgia::visualiser {

	namespace {
		std::vector<BenchmarkPlotData> current_benchmark_plot_data;
        std::unordered_map<std::string, std::vector<BenchmarkPlotData>> current_benchmark_plot_data_map;

		int current_hovered_index;
		BenchmarkPlotData* current_hovered_data = nullptr;

        // New Storage for displayed benchmark results
        std::vector<BenchmarkResults> displayed_benchmark_results;

        std::string reference_benchmark_path = "reference_benchmark_results.json";
	}

    size_t get_total_displayed_benchmark_results() {
		return displayed_benchmark_results.size();
	}

    std::string get_benchmark_label(size_t index) {
		if (index < 0 || index >= get_total_displayed_benchmark_results()) return "";
		return displayed_benchmark_results[index].benchmark_label;
	}

    std::unordered_map<std::string, std::vector<BenchmarkPlotData>> load_results_from_file(const std::string& path, ImportMapKey map_key, ImportMapKey import_filter_for, std::string benchmark_filter) {

        log::print("Loading benchmark results from: {}", path);

        if (path.empty()) {
            log::print(LogFlags::Error, "Path is empty, cannot load benchmark results.");
            return {};
        }
        if (!std::filesystem::exists(path)) {
            log::print(LogFlags::Error, "File does not exist: {}", path);
            return {};
        }
        
        std::ifstream file(path);
        json j;
        file >> j;

        std::unordered_map<std::string, std::vector<BenchmarkPlotData>> results;

        for (const auto& entry : j) {
            switch(import_filter_for) {
                case ImportMapKey::NONE:
                    break;
                case ImportMapKey::BenchmarkLabel:
                    if (entry["benchmark_label"] != benchmark_filter) continue;
                    break;
                case ImportMapKey::ResultsSource:
                    if (entry["results_source"] != benchmark_filter) continue;
                    break;
                default:
                    log::print(LogFlags::Error, "Unknown ImportMapKey: %d", static_cast<int>(import_filter_for));
                    continue;
            }
            std::string benchmark_label = entry["benchmark_label"];
            std::string results_source = entry["results_source"];
            const auto& res = entry["results"];

            BenchmarkPlotData d;

			d.benchmark_label = benchmark_label;
            d.results_source = results_source;

            d.allocator_label = res["allocator_label"];
            d.allocator_description = res["allocator_description"];
            d.implementation_label = res["implementation_label"];
            d.implementation_description = res["implementation_description"];
            d.implementation_parameters = res["implementation_parameters"];
            d.total_time = res["total_time"];
            d.allocate_time = res["allocate_time"];
            d.deallocate_time = res["deallocate_time"];
            d.iterations = res["iterations"];
            d.passes = res["passes"];

            switch (map_key) {
                case ImportMapKey::BenchmarkLabel:
                    results[benchmark_label].push_back(d);
                    break;
                case ImportMapKey::ResultsSource:
                    results[results_source].push_back(d);
                    break;
                default:
                    log::print(LogFlags::Error, "Unknown ImportMapKey: %d", static_cast<int>(map_key));
                    break;
            }
        }
        return results;
    }

    ReturnCode load_reference_benchmark_results(std::string benchmark_label, BenchmarkResults& loaded_results) {
        log::print("Loading reference benchmark results from: {}", reference_benchmark_path);

        std::unordered_map<std::string, std::vector<BenchmarkPlotData>> reference_benchmark_data = load_results_from_file(reference_benchmark_path, ImportMapKey::ResultsSource, ImportMapKey::BenchmarkLabel);


        if (reference_benchmark_data.empty()) {
            log::print(LogFlags::Warn, "No reference benchmark results found in file: {}", reference_benchmark_path);
            return ReturnCode::ImportError;
        }
        // Check if the benchmark label exists in the reference data
        if (!reference_benchmark_data.contains(benchmark_label)) {
            log::print(LogFlags::Warn, "No reference benchmark results found for label: {}", benchmark_label);
            return ReturnCode::ImportError;
        }

        loaded_results.reference_results = 
            reference_benchmark_data;
        
        return ReturnCode::Success;
    }
        

    void load_local_benchmark_results(std::string path) {
        
        BenchmarkResults loaded_results;
        log::print("Loading local benchmark results from: {}", path);

        std::unordered_map<std::string, std::vector<BenchmarkPlotData>> loaded_plot_data = load_results_from_file(path, ImportMapKey::BenchmarkLabel);

        if (loaded_plot_data.empty()) {
            log::print(LogFlags::Warn, "No benchmark results found in file: {}", path);
            return;
        }

        // Currently only supporting one benchmark label at a time
        loaded_results.benchmark_label = loaded_plot_data.begin()->first;
        loaded_results.local_results = loaded_plot_data[loaded_results.benchmark_label];

        // Add reference results if available
        load_reference_benchmark_results(loaded_results.benchmark_label, loaded_results);

        // Add to the displayed results vector
        displayed_benchmark_results.push_back(loaded_results);
    }



    /// Old below here, needs to be updated to use the new data structure
    static std::vector<std::string> labelStorage;
    static std::vector<const char*> labels;

    void draw_benchmark_plot(std::unordered_map<std::string, std::vector<BenchmarkPlotData>>&data) {
        if (ImGui::BeginTabBar("BenchmarkTabs")) {
            for (auto& [benchmark_label, entries] : data) {
                if (ImGui::BeginTabItem(benchmark_label.c_str())) {
                    draw_benchmark_plot(entries);
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
    }

    void draw_benchmark_results_view(size_t index) {
        draw_benchmark_plot(displayed_benchmark_results[index].local_results);
    }

    void display_hover_details(int index) {
        current_hovered_index = index;
    }

    void draw_benchmark_hover_details() {
        // ImGui::Text("Benchmark:");

        if (current_hovered_data)  ImGui::TextWrapped("Benchmark: %s", current_hovered_data->benchmark_label.c_str());

        ImGui::Text("Benchmark Details:");

        if (current_hovered_data)  ImGui::TextWrapped("%s", current_hovered_data->benchmark_description.c_str());

        ImGui::Text("Benchmark Parameters:");

        if (current_hovered_data)  ImGui::TextWrapped("%d Passes of ", static_cast<int>(current_hovered_data->passes));
        if (current_hovered_data)  ImGui::TextWrapped("%d Iterations", static_cast<int>(current_hovered_data->iterations));

        ImGui::Separator();

        ImGui::Text("Allocator:");

        if (current_hovered_data)  ImGui::TextWrapped("%s", current_hovered_data->allocator_label.c_str());

        ImGui::Text("Allocator Details:");

        if (current_hovered_data)  ImGui::TextWrapped("%s", current_hovered_data->allocator_description.c_str());

        ImGui::Separator();

        ImGui::Text("Implementation:");

        if (current_hovered_data)  ImGui::TextWrapped("%s", current_hovered_data->implementation_label.c_str());

        ImGui::Text("Implementation Details:");

        if (current_hovered_data)  ImGui::TextWrapped("%s", current_hovered_data->implementation_description.c_str());
        if (current_hovered_data)  ImGui::TextWrapped("%s", current_hovered_data->implementation_parameters.c_str());
        
        ImGui::Separator();

        ImGui::Text("Results:");

        ImGui::Text("Hovered Results:");

        if (current_hovered_data)  ImGui::Text("Total Time: %.3f ms", current_hovered_data->total_time);
        if (current_hovered_data)  ImGui::Text("Allocation Time: %.3f ms", current_hovered_data->allocate_time);
        if (current_hovered_data)  ImGui::Text("Deallocation Time: %.3f ms", current_hovered_data->deallocate_time);
    }

    void draw_benchmark_plot(const std::vector<BenchmarkPlotData>& data) {

        if (ImPlot::BeginPlot("Benchmark Results Grouped By Allocator Type", ImVec2(-1, -1), ImPlotFlags_NoMouseText)) {

            // UI Storage for labels
            //std::vector<std::string> labelStorage;
            //std::vector<const char*> labels;
            std::vector<std::string> hoverStorage;
			std::vector<int> hoveredRealIndices; 

            labelStorage.clear();
            labels.clear();

            // Data storage for bars
            std::vector<double> allocs, deallocs, totals;

            int letterIndex = 0;
            int hoveredRealIndex = 0;
            std::string lastAllocator; // Only works if allocators are ordered

            for (const auto& d : data) {
                if (!lastAllocator.empty() && lastAllocator != d.allocator_label) {
                    labelStorage.emplace_back("");      
                    hoverStorage.emplace_back("");      
                    allocs.push_back(0);
                    deallocs.push_back(0);
                    totals.push_back(0);
                    hoveredRealIndices.push_back(-1);
                }
                lastAllocator = d.allocator_label;

                // Push bar values
                allocs.push_back(d.allocate_time);
                deallocs.push_back(d.deallocate_time);
                totals.push_back(d.total_time);

                // Label with Letters only
                std::string shortLabel(1, 'A' + (char)letterIndex++);
                labelStorage.push_back(std::move(shortLabel));           

				hoveredRealIndices.push_back(hoveredRealIndex++); // Store real index for hover
				hoverStorage.push_back(d.allocator_label + ": " + d.implementation_label); // full name for tooltip
            }

            std::vector<const char*> labelPtrs;
            labelPtrs.reserve(labelStorage.size());
            for (const auto& s : labelStorage)
                labelPtrs.push_back(s.c_str());
            int count = (int)allocs.size();

            ImPlot::SetupAxes(nullptr, "Time (ms)");
            //ImPlot::SetupAxisTicks(ImAxis_X1, 0, count - 1, count, labelStorage.data());
            ImPlot::SetupAxisTicks(ImAxis_X1, 0, count - 1, count, labelPtrs.data());

            ImPlot::PlotBars("Alloc", allocs.data(), count, 0.25, -0.25);
            ImPlot::PlotBars("Dealloc", deallocs.data(), count, 0.25, 0.25);
            ImPlot::PlotBars("Total", totals.data(), count, 0.25, 0.0);
            
            if (ImPlot::IsPlotHovered()) {
                ImPlotPoint mouse = ImPlot::GetPlotMousePos();
                int hoveredIndex = (int)(mouse.x + 0.5);

                if (hoveredIndex >= 0 && hoveredIndex < (int)labelStorage.size() && hoverStorage[static_cast<size_t>(hoveredIndex)].length() > 0) {
                    const char* tooltipLabel = hoverStorage[static_cast<size_t>(hoveredIndex)].c_str(); // full label is stored before short
                    if (tooltipLabel[0] != '\0') {
                        ImGui::BeginTooltip();
                        ImGui::TextUnformatted(tooltipLabel);
                        ImGui::EndTooltip();
                        // Need to store this index to then pass and use to reference the rest of the data.
                        display_hover_details(hoveredIndex);
                        current_hovered_data = const_cast<BenchmarkPlotData*>(&data[static_cast<size_t>(hoveredRealIndices[static_cast<size_t>(hoveredIndex)])]);
                    }
                }
            }

            ImPlot::EndPlot();
        }
    }
}