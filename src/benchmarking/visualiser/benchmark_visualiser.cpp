#include "benchmark_visualiser.h"

#include <nlohmann/json.hpp>
#include <fstream>

#include <imgui.h>
#include <implot.h>
#include "implot_internal.h"


using json = nlohmann::json;

namespace nostalgia::visualiser {

    // Current
    std::unordered_map<std::string, std::vector<BenchmarkPlotData>> loadResultsFromFile(const std::string& path) {
        std::ifstream file(path);
        json j;
        file >> j;

        std::unordered_map<std::string, std::vector<BenchmarkPlotData>> results;

        for (const auto& entry : j) {
            std::string benchmarkLabel = entry["benchmarkLabel"];
            const auto& res = entry["results"];

            BenchmarkPlotData d;

			d.benchmarkLabel = benchmarkLabel;

            d.allocatorLabel = res["allocatorLabel"];
            d.allocatorDescription = res["allocatorDescription"];
            d.implementationLabel = res["implementationLabel"];
            d.implementationDesc = res["implementationDesc"];
            d.implementationParameters = res["implementationParameters"];
            d.totalTime = res["totalTime"];
            d.allocateTime = res["allocateTime"];
            d.deallocateTime = res["deallocateTime"];
            d.iterations = res["iterations"];
            d.passes = res["passes"];

            results[benchmarkLabel].push_back(d);
        }

        return results;
    }

    // Deprecated
    std::vector<BenchmarkPlotData> loadBenchmarkPlotData(const std::string& path) {
        std::ifstream file(path);
        json j;
        file >> j;

        std::vector<BenchmarkPlotData> data;
        for (const auto& entry : j) {
            const auto& res = entry["results"];
            BenchmarkPlotData d;
            d.implementationLabel = res["implementationLabel"];
            d.allocatorLabel = res["allocatorLabel"];
            d.totalTime = res["totalTime"];
            d.allocateTime = res["allocateTime"];
            d.deallocateTime = res["deallocateTime"];
            data.push_back(d);
        }
        return data;
    }

    void loadBenchmarkPlotData() {
        m_currentPlotData = loadBenchmarkPlotData("benchmark_results.txt");
		m_benchmarkPlotDataMap = loadResultsFromFile("benchmark_results.txt");

        if (m_benchmarkPlotDataMap.empty()) ImGui::Text("No benchmark map loaded.");
        if (m_currentPlotData.empty()) ImGui::Text("No benchmark data loaded.");
    }


    static std::vector<std::string> labelStorage;
    static std::vector<const char*> labels;

    void ShowBenchmarkPlot(std::unordered_map<std::string, std::vector<BenchmarkPlotData>>&data) {
        if (ImGui::BeginTabBar("BenchmarkTabs")) {
            for (auto& [benchmarkLabel, entries] : data) {
                if (ImGui::BeginTabItem(benchmarkLabel.c_str())) {
                    ShowBenchmarkPlot(entries);
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
    }

    void ShowBenchmarkPlot() {
        if (!m_benchmarkPlotDataMap.empty()) {
            ShowBenchmarkPlot(m_benchmarkPlotDataMap);
        }
    }

    void DisplayHoverDetails(int index) {
        m_hoveredIndex = index;
    }

    void ShowDetails() {
        // ImGui::Text("Benchmark:");

        if (m_hoveredData)  ImGui::TextWrapped("Benchmark: %s", m_hoveredData->benchmarkLabel.c_str());

        ImGui::Text("Benchmark Details:");

        if (m_hoveredData)  ImGui::TextWrapped("%s", m_hoveredData->benchmarkDesc.c_str());

        ImGui::Text("Benchmark Parameters:");

        if (m_hoveredData)  ImGui::TextWrapped("%d Passes of ", m_hoveredData->passes);
        if (m_hoveredData)  ImGui::TextWrapped("%d Iterations", m_hoveredData->iterations);

        ImGui::Separator();

        ImGui::Text("Allocator:");

        if (m_hoveredData)  ImGui::TextWrapped("%s", m_hoveredData->allocatorLabel.c_str());

        ImGui::Text("Allocator Details:");

        if (m_hoveredData)  ImGui::TextWrapped("%s", m_hoveredData->allocatorDescription.c_str());

        ImGui::Separator();

        ImGui::Text("Implementation:");

        if (m_hoveredData)  ImGui::TextWrapped("%s", m_hoveredData->implementationLabel.c_str());

        ImGui::Text("Implementation Details:");

        if (m_hoveredData)  ImGui::TextWrapped("%s", m_hoveredData->implementationDesc.c_str());
        if (m_hoveredData)  ImGui::TextWrapped("%s", m_hoveredData->implementationParameters.c_str());
        
        ImGui::Separator();

        ImGui::Text("Results:");

        ImGui::Text("Hovered Results:");

        if (m_hoveredData)  ImGui::Text("Total Time: %.3f ms", m_hoveredData->totalTime);
        if (m_hoveredData)  ImGui::Text("Allocation Time: %.3f ms", m_hoveredData->allocateTime);
        if (m_hoveredData)  ImGui::Text("Deallocation Time: %.3f ms", m_hoveredData->deallocateTime);
    }

    void ShowBenchmarkPlot(const std::vector<BenchmarkPlotData>& data) {

        if (ImPlot::BeginPlot("Benchmark Results Grouped By Allocator Type", ImVec2(-1, -1), ImPlotFlags_NoMouseText)) {

            // UI Storage for labels
            //std::vector<std::string> labelStorage;
            //std::vector<const char*> labels;
            std::vector<std::string> hoverStorage;
			std::vector<int> hoveredRealIndices; 

            labelStorage.clear();
            labels.clear();

            // Data storage for bars
            std::vector<float> allocs, deallocs, totals;

            int letterIndex = 0;
            int hoveredRealIndex = 0;
            std::string lastAllocator; // Only works if allocators are ordered

            for (const auto& d : data) {
                if (!lastAllocator.empty() && lastAllocator != d.allocatorLabel) {
                    labelStorage.emplace_back("");      
                    hoverStorage.emplace_back("");      
                    allocs.push_back(0);
                    deallocs.push_back(0);
                    totals.push_back(0);
                    hoveredRealIndices.push_back(-1);
                }
                lastAllocator = d.allocatorLabel;

                // Push bar values
                allocs.push_back(d.allocateTime);
                deallocs.push_back(d.deallocateTime);
                totals.push_back(d.totalTime);

                // Label with Letters only
                std::string shortLabel(1, 'A' + (char)letterIndex++);
                labelStorage.push_back(std::move(shortLabel));           

				hoveredRealIndices.push_back(hoveredRealIndex++); // Store real index for hover
				hoverStorage.push_back(d.allocatorLabel + ": " + d.implementationLabel); // full name for tooltip
            }

            std::vector<const char*> labelPtrs;
            labelPtrs.reserve(labelStorage.size());
            for (const auto& s : labelStorage)
                labelPtrs.push_back(s.c_str());
            int count = (int)allocs.size();

            ImPlot::SetupAxes(nullptr, "Time (ms)");
            //ImPlot::SetupAxisTicks(ImAxis_X1, 0, count - 1, count, labelStorage.data());
            ImPlot::SetupAxisTicks(ImAxis_X1, 0, count - 1, count, labelPtrs.data());

            ImPlot::PlotBars("Alloc", allocs.data(), count, 0.25f, -0.25f);
            ImPlot::PlotBars("Dealloc", deallocs.data(), count, 0.25f, 0.25f);
            ImPlot::PlotBars("Total", totals.data(), count, 0.25f, 0.0f);
            
            if (ImPlot::IsPlotHovered()) {
                ImPlotPoint mouse = ImPlot::GetPlotMousePos();
                int hoveredIndex = (int)(mouse.x + 0.5f);

                if (hoveredIndex >= 0 && hoveredIndex < (int)labelStorage.size() && hoverStorage[hoveredIndex].length() > 0) {
                    const char* tooltipLabel = hoverStorage[hoveredIndex].c_str(); // full label is stored before short
                    if (tooltipLabel[0] != '\0') {
                        ImGui::BeginTooltip();
                        ImGui::TextUnformatted(tooltipLabel);
                        ImGui::EndTooltip();
                        // Need to store this index to then pass and use to reference the rest of the data.
                        DisplayHoverDetails(hoveredIndex);
                        m_hoveredData = const_cast<BenchmarkPlotData*>(&data[hoveredRealIndices[hoveredIndex]]);
                    }
                }
            }

            ImPlot::EndPlot();
        }
    }


}