#include "gui.h"

#include "render.h"
#include "style.h"

#include "allocators/allocator_meta.h"
#include "allocators/linear_bump/allocator_linear.h"

#include "benchmarking/benchmarking_manager.h"
#include "benchmarking/exporting/benchmarking_exporting.h"
#include "benchmarking/benchmark_atlas.h"

#include "benchmarking/visualiser/benchmark_visualiser.h"

#include "benchmarking/benchmark_loader.h"

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include "imgui.h"

#include "benchmarking/benchmark_params.h"

#include "log.h"

namespace nostalgia::gui{

    namespace {
		// Layout Settings
		bool g_leftPanelOpen = true;
		bool g_topBarOpen = true;

		// Layout ~Constants
		const float sidePanelWidth = 300.0f; 
		const float sidePanelButtonHeight = 30.0f;
		const float topBarHeight = 300.0; 
		const float mainPageRightPanelWidth = 400.0f; // Width of the right panel on the main page

		// Parameter Inputs
		std::unordered_map<std::string, std::string> stringInputs;
		std::unordered_map<std::string, int> intInputs;
		std::unordered_map<std::string, bool> boolInputs;

		const std::vector<BenchmarkParamSpec>* m_paramSpecs = nullptr; 
		nostalgia::BenchmarkParams* m_parameters = nullptr;
    }

	void load_benchmarking_params(const std::vector<BenchmarkParamSpec>& specs) {
		log::print(logFlags::DEBUG, "Params loaded into m_parameters: {} keys", specs.size());

		m_paramSpecs = &specs;
		m_parameters = &nostalgia::benchmarking::loader::getParameters();


		log::print("[PARAM] Assigned m_parameters = {}", static_cast<const void*>(&m_parameters));

		for (const auto& spec : specs) {

			if (spec.typeName == "int") {
				log::print(logFlags::DEBUG, "Loading int parameter: {}", spec.key);
				intInputs[spec.key] = spec.defaultValue ? std::stoi(*spec.defaultValue) : 0;
			} else if (spec.typeName == "string") {
				stringInputs[spec.key] = spec.defaultValue ? *spec.defaultValue : "";
			} else if (spec.typeName == "bool") {
				boolInputs[spec.key] = spec.defaultValue ? (*spec.defaultValue == "true") : false;
			}
		}
	}

	// Per-Frame GUI Entry Point
    void draw_gui() {
		style::withExpandedWindow("Nostalgia", []() {

			// Left Panel
			if (g_leftPanelOpen) draw_leftPanel();

			ImGui::SameLine();

			style::withChildWrapper("Main Content", ImVec2(0, 0), []() {
				// Top Bar
				if (g_topBarOpen) draw_topBar();

				ImGui::Spacing();

				// Main Content
				draw_mainPage();

			}, false);
        });
    };

	// Left Panel - For Benchmark Selection
	void draw_leftPanel() {
		ImVec2 _sidebarStartPos = ImGui::GetCursorScreenPos();

		style::withChildWrapper("Side Panel", ImVec2(sidePanelWidth, 0), []() {
			if (ImGui::ArrowButton("##toggleSidePanel", g_leftPanelOpen ? ImGuiDir_Left : ImGuiDir_Right)) {
				g_leftPanelOpen = !g_leftPanelOpen;
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			bool first = true;

			for (const auto& [bID, bType] : nostalgia::benchmark::atlas) {
			
				if (bType.disabled) ImGui::BeginDisabled();

				style::drawWideButton(
					bType.label.c_str(), 
					0.9f, 
					sidePanelButtonHeight, 
					[&bID](){nostalgia::benchmarking::loader::loadBenchmark(bID);});

				if (first) {
					first = false;
					ImGui::Spacing();
					ImGui::Separator();
				}
				if (bType.disabled) ImGui::EndDisabled();
				ImGui::Spacing();
			}

			style::drawWideButton("Export Benchmarks", 0.9f, sidePanelButtonHeight, []() {
				nostalgia::benchmarking::exporting::exportCurrentBenchmarks();
				});

			style::drawWideButton("Visualise Benchmarks", 0.9f, sidePanelButtonHeight, []() {
				nostalgia::visualiser::loadBenchmarkPlotData();
				});
		});
	}

	// Top Bar - Trigger Benchmarks, Implementation Setup, Details, and Hover Descriptions
    void draw_topBar() {
		ImVec2 _topbarStartPos = ImGui::GetCursorScreenPos();
		if (!m_paramSpecs || !m_parameters) return;
		style::withChildWrapper("Top Bar", ImVec2(0, topBarHeight), []() {

			// Parameter Input
			//static std::unordered_map<std::string, std::string> stringInputs;
			//static std::unordered_map<std::string, int> intInputs;
			//static std::unordered_map<std::string, bool> boolInputs;
				
//void ShowBenchmarkParamEditor(const std::vector<BenchmarkParamSpec>& specs, nostalgia::BenchmarkParams& outputParams) {
		for (const auto& spec : *m_paramSpecs) {
			const std::string& key = spec.key;
			const std::string& type = spec.typeName;

			ImGui::Text("%s", spec.description.c_str());

			if (type == "int") {
				int& value = intInputs[key];
				if (ImGui::InputInt(key.c_str(), &value))
					m_parameters->set<int>(key, value);				
			}
			else if (type == "string") {
				std::string& buf = stringInputs[key];
				if (buf.empty() && spec.defaultValue) buf = *spec.defaultValue;

				char input[256];
				strncpy(input, buf.c_str(), sizeof(input));
				input[sizeof(input) - 1] = 0;

				if (ImGui::InputText(key.c_str(), input, sizeof(input))) {
					buf = input;
				}

				m_parameters->set<std::string>(key, buf);
			}
			else if (type == "bool") {
				bool& value = boolInputs[key];
				ImGui::Checkbox(key.c_str(), &value);
				m_parameters->set<bool>(key, value);
			}
			else {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "Unsupported type: %s", type.c_str());
			}
		}
		//log::print("m_parameters pointer: {}", reinterpret_cast<uintptr_t>(m_parameters));

		ImGui::Separator();

		if (ImGui::Button("Run Benchmark")) {
			// Run the benchmark with the current parameters
			// Temp hardcoded for now
			nostalgia::benchmarking::loader::dispatchBenchmark(nostalgia::BenchmarkID::IBM_Bursts);
			nostalgia::benchmarking::exporting::exportCurrentBenchmarks();
			nostalgia::visualiser::loadBenchmarkPlotData();
		}


		});
    }

	// Main Page - Results Graphs
    void draw_mainPage() {
		ImVec2 _mainStartPos = ImGui::GetCursorScreenPos();
		//log::print("Drawing main panel at [{}, {}]", _mainStartPos.x, _mainStartPos.y);
		
		style::withChildWrapper("Main Page", ImVec2(0, 0), []() {
			
			float _availWidth = ImGui::GetContentRegionAvail().x;
			float _panelWidth = std::max(_availWidth - mainPageRightPanelWidth, _availWidth * 0.66f);
			style::withChildWrapper("Main Page Content", ImVec2(_panelWidth, 0), []() {

			// ImGui::Text("Main Page");

			visualiser::ShowBenchmarkPlot();

			});

			ImGui::SameLine();

			style::withChildWrapper("Main Page Details", ImVec2(0, 0), []() {

				ImGui::Text("Main Page Details");
				visualiser::ShowDetails();

			});
		}, false);
    }

	// Initialises SDL-Vulkan-DearIMGUI backend, calls draw_gui() every frame
	int init_gui() {
		init_renderBackend();
		return INIT_OK;
	};

}