#include "benchmark_visualiser.h"

//#include <nlohmann/json.hpp>
#include "utils/json_wrapped.h"
#include <fstream>

#include <imgui.h>

#include "ui/system/style.h"

#include "utils/implot_wrapped.h"

#include "utils/log.h"
#include "utils/return_codes.h"

#include "allocators/info/allocator_atlas.h"

using json = nlohmann::json;

namespace nostalgia::visualiser {

	namespace {
		std::vector<BenchmarkPlotData> current_benchmark_plot_data;
        std::unordered_map<std::string, std::vector<BenchmarkPlotData>> current_benchmark_plot_data_map;
        std::string reference_benchmark_path = "reference_benchmark_results.json";

		int current_hovered_index;
		BenchmarkPlotData* current_hovered_data = nullptr;
        std::vector<BenchmarkResults> displayed_benchmark_results;

        enum class ResultsHoverTab {
            Overview = 0,
            Details = 1,
            View = 2,
            Export = 3,
            COUNT = 4
        };

        constexpr std::size_t tab_count = static_cast<std::size_t>(ResultsHoverTab::COUNT);
        constexpr std::array<const char*, tab_count> results_hover_tab_names = {
            "Overview",
            "Details",
            "View",
            "Export"
        };
        static_assert(results_hover_tab_names.size() == tab_count,
            "ResultsHoverTab enum size does not match names array size.");

		ResultsHoverTab current_hovered_tab = ResultsHoverTab::Overview;

		constexpr float     hover_tab_max_width_perc        = 1.0f / static_cast<float>(tab_count);
		constexpr float     hover_tab_height                = 30.0f;
        constexpr ImVec2 	expandable_size                 = ImVec2(0.0f, 0.0f);
        constexpr float 	expandable_value                = 0.0f;

		// Visible Graph data storage
        std::vector<std::string> labelStorage;
        std::vector<const char*> labels;

        enum class ColorBy {
            NONE = 0,
            AllocatorType,
            ImplementationType,
            COUNT
		};
		ColorBy current_color_by = ColorBy::NONE;

        constexpr float     wrap_color_8bit                = 1.0f / 256.0f;
        constexpr float     plot_bar_opacity                = 1.0f;
        constexpr ImVec4    default_hue_color               = ImVec4(1.0f, 0.0f, 0.0f, plot_bar_opacity);

        constexpr ImVec4    alloc_time_color                = ImVec4(1.0f, 0.0f, 0.0f, plot_bar_opacity);
        constexpr ImVec4    dealloc_time_color              = ImVec4(0.0f, 1.0f, 0.0f, plot_bar_opacity);
        constexpr ImVec4    total_time_color                = ImVec4(0.0f, 0.0f, 1.0f, plot_bar_opacity);
        constexpr float     alloc_time_shading              = 0.75f;
        constexpr float     dealloc_time_shading            = 0.5f;
        constexpr float     total_time_shading              = 1.0f;

        enum class ResultDataType {
            AllocTime,
            DeallocTime,
            TotalTime
		};
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

    void draw_benchmark_results_hover_overview() {
        ImGui::Text("Benchmark Results Overview");
        ImGui::Separator();
        if (current_hovered_data) {
            ImGui::Text("Benchmark: %s", current_hovered_data->benchmark_label.c_str());
            ImGui::Text("Allocator: %s", current_hovered_data->allocator_label.c_str()); 
            ImGui::Text("Implementation: %s", current_hovered_data->implementation_label.c_str());
            ImGui::Text("Total Time: %.3f ms", current_hovered_data->total_time);
            ImGui::Text("Allocation Time: %.3f ms", current_hovered_data->allocate_time);
            ImGui::Text("Deallocation Time: %.3f ms", current_hovered_data->deallocate_time);

            // Parameter Info
            

            // Top 3 Local Allocators


            // Top 3 Reference Allocators


            // Top 3 Implementations


			// Top 3 Reference Implementations


			// Overall Performance Summary
            // Allocation  / Total Time Ratio
            // Deallocation  / Total Time Ratio

            // Gains from worst to best
            // Gains from average to best


        } else {
            ImGui::Text("No benchmark data hovered.");
        }
	}

    void draw_benchmark_results_hover_details() {
        ImGui::Text("Benchmark Details");
        ImGui::Separator();
        if (current_hovered_data) {
            ImGui::Text("Benchmark: %s", current_hovered_data->benchmark_label.c_str());
            ImGui::Text("Description: %s", current_hovered_data->benchmark_description.c_str());
            ImGui::Text("Parameters: %d Passes, %d Iterations", static_cast<int>(current_hovered_data->passes), static_cast<int>(current_hovered_data->iterations));
            ImGui::Text("Allocator: %s", current_hovered_data->allocator_label.c_str());
            ImGui::Text("Allocator Details: %s", current_hovered_data->allocator_description.c_str());
            ImGui::Text("Implementation: %s", current_hovered_data->implementation_label.c_str());
            ImGui::Text("Implementation Details: %s", current_hovered_data->implementation_description.c_str());
            ImGui::Text("Implementation Parameters: %s", current_hovered_data->implementation_parameters.c_str());
        } else {
            ImGui::Text("No benchmark data hovered.");
		}
    }

    void draw_benchmark_results_hover_view() {

        ImGui::Spacing();
        ImGui::Text("Color by...");
        if (ImGui::Button("Allocator Type")) current_color_by = ColorBy::AllocatorType;
        if (ImGui::Button("Implementation Type")) current_color_by = ColorBy::ImplementationType;
    }

    void draw_benchmark_results_hover_export() {

    }

    void draw_benchmark_results_hover() {

        // ~~~ Tabs ~~~
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, expandable_size);
		float available_width = ImGui::GetContentRegionAvail().x;
		float hover_tab_button_width = available_width * hover_tab_max_width_perc - ImGui::GetStyle().ItemSpacing.x * (tab_count - 1);
        for (size_t i = 0; i < tab_count; ++i) {
			ImGui::PushID(static_cast<int>(i));
            ImGui::SameLine();
            if (nostalgia::gui::style::draw_tab_button(results_hover_tab_names[i], (current_hovered_tab == static_cast<ResultsHoverTab>(i)), ImVec2(hover_tab_button_width, hover_tab_height))) {
                current_hovered_tab = static_cast<ResultsHoverTab>(i);
			}
            ImGui::PopID();
		}
        ImGui::PopStyleVar();

        ImGui::Spacing();

        switch (current_hovered_tab) {
            case ResultsHoverTab::Overview:
                draw_benchmark_results_hover_overview();
                break;
            case ResultsHoverTab::Details:
                draw_benchmark_results_hover_details();
                break;
            case ResultsHoverTab::View:
                draw_benchmark_results_hover_view();
                break;
            case ResultsHoverTab::Export:
                draw_benchmark_results_hover_export();
                break;
            default:
                ImGui::Text("Unknown tab selected.");
                break;
		}
    }

    ImVec4 color_mix(const ImVec4& a, const ImVec4& b, float factor = 0.5f, float brightness = 1.0f) {
        return ImVec4(
            (a.x * (1.0f - factor) + b.x * factor) * brightness,
            (a.y * (1.0f - factor) + b.y * factor) * brightness,
            (a.z * (1.0f - factor) + b.z * factor)* brightness,
            a.w * (1.0f - factor) + b.w * factor
        );
	}
    
    ImVec4 get_allocator_color(AllocatorID a_id) {
        float h, s, v;
        ImGui::ColorConvertRGBtoHSV(default_hue_color.x, default_hue_color.y, default_hue_color.z, h, s, v);
		h = fmodf(h + static_cast<float>(nostalgia::allocator::get_color_hue_shift(a_id)) * wrap_color_8bit, 1.0f);
        float r, g, b; 
		ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
		return ImVec4(r, g, b, plot_bar_opacity);
    }

    ImVec4 get_allocator_color(const std::string& allocator_label) {
        for (const auto& [a_id, a_type] : allocator::atlas) {
            if (a_type.label == allocator_label) {
                return get_allocator_color(a_id);
            }
		}
        return default_hue_color;
	}

    ImVec4 get_color_by_result(ResultDataType data_type, BenchmarkPlotData data) {
		
        ImVec4 base_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		float mix_factor = 0.5f; 
        bool mix_brightness = false;

        switch (current_color_by) {
        case ColorBy::NONE:
            mix_factor = 1.0f;
            break;
        case ColorBy::AllocatorType:
            base_color = get_allocator_color(data.allocator_label);
            mix_brightness = true;
            mix_factor = 0.0f;
            break;
		case ColorBy::ImplementationType:
            //break;
        default:
            break;
		}

        switch (data_type) {
        case ResultDataType::AllocTime: return color_mix(base_color, alloc_time_color, mix_factor, (mix_brightness ? alloc_time_shading : 1.0f));
        case ResultDataType::DeallocTime: return color_mix(base_color, dealloc_time_color, mix_factor, (mix_brightness ? dealloc_time_shading : 1.0f));
        case ResultDataType::TotalTime: return color_mix(base_color, total_time_color, mix_factor, (mix_brightness ? total_time_shading : 1.0f));
        default:
                log::print(LogFlags::Error, "Unknown ResultDataType: %d", static_cast<int>(data_type));
				return base_color; // Fallback to default color
        }
    }

    void draw_benchmark_plot(const std::vector<BenchmarkPlotData>& data) {

        if (ImPlot::BeginPlot("Benchmark Results Grouped By Allocator Type", ImVec2(-1, -1), ImPlotFlags_NoMouseText)) {

            // UI Storage for labels
            std::vector<std::string> hoverStorage;
			std::vector<int> hoveredRealIndices; 

            labelStorage.clear();
            labels.clear();

            // Data storage for bars
            std::vector<double> allocs, deallocs, totals;
            std::vector<AllocatorID> a_id;

            int letterIndex = 0;
            int hoveredRealIndex = 0;
            // std::string lastAllocator; // Only works if allocators are ordered

            //for (const BenchmarkPlotData& d : data) {
            for (size_t i = 0; i < data.size(); ++i) {

                const BenchmarkPlotData& d = data[i];
                // Change this to  record an offset index instead - and just increment that
                // and use it to move the real indices without getting offset
                // Implement GroupBy to handle this sorting logic
                
                // Old Forced Spacing for allocator_label differences
                /*if (!lastAllocator.empty() && lastAllocator != d.allocator_label) {
                    labelStorage.emplace_back("");      
                    hoverStorage.emplace_back("");      
                    allocs.push_back(0);
                    deallocs.push_back(0);
                    totals.push_back(0);
                    hoveredRealIndices.push_back(-1);
                }
                lastAllocator = d.allocator_label;*/

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

            ImPlot::SetupAxes(nullptr, "Time (ms)", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
            //ImPlot::SetupAxisTicks(ImAxis_X1, 0, count - 1, count, labelStorage.data());
            ImPlot::SetupAxisTicks(ImAxis_X1, 0, count - 1, count, labelPtrs.data());

            for (size_t i = 0; i < count; ++i) {

                ImPlot::SetNextFillStyle(get_color_by_result(ResultDataType::AllocTime, data[i]));
                ImPlot::PlotBars("Alloc", &allocs[i], 1, 0.25, static_cast<double>(i) - 0.25);
                ImPlot::SetNextFillStyle(get_color_by_result(ResultDataType::TotalTime, data[i]));
                ImPlot::PlotBars("Total", &totals[i], 1, 0.25, static_cast<double>(i));
                ImPlot::SetNextFillStyle(get_color_by_result(ResultDataType::DeallocTime, data[i]));
                ImPlot::PlotBars("Dealloc", &deallocs[i], 1, 0.25, static_cast<double>(i) + 0.25);
            }

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