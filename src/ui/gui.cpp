#include "gui.h"

#include "render.h"
#include "style.h"

#include "allocators/allocator_meta.h"
#include "allocators/allocator_atlas.h"
#include "allocators/linear_bump/allocator_linear.h"

#include "benchmarking/benchmarking_manager.h"
#include "benchmarking/exporting/benchmarking_exporting.h"
#include "benchmarking/benchmark_atlas.h"

#include "benchmarking/visualiser/benchmark_visualiser.h"

#include "benchmarking/benchmark_loader.h"

#include "implementations/implementation_atlas.h"

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

		// Allocator Selection
		ImVec2 allocatorButtonSize = ImVec2(120, 0); // Size of the allocator selection buttons

		// Results View
		int m_currentResultTab = -1; 

		enum class HoverDetailState {
			NONE,
			allocator,
			implementation,
			parameter,
			run
		};
		nostalgia::ImplementationID m_hoveredImplementation = nostalgia::ImplementationID::NONE;
		nostalgia::AllocatorID m_hoveredAllocator = nostalgia::AllocatorID::Linear;
		const nostalgia::BenchmarkParamSpec* m_hoveredParameter = nullptr;
		HoverDetailState m_hoverState = HoverDetailState::NONE;

		const std::vector<BenchmarkParamSpec>* m_paramSpecs = nullptr; 
		nostalgia::BenchmarkParams* m_parameters = nullptr;
    }

	void load_benchmarking_params(const std::vector<BenchmarkParamSpec>& specs) {
		log::print(logFlags::DEBUG, "Params loaded into m_parameters: {} keys", specs.size());

		m_paramSpecs = &specs;
		m_parameters = &nostalgia::benchmarking::loader::getParameters();


		log::print("[PARAM] Assigned m_parameters = {}", static_cast<const void*>(&m_parameters));

		for (const BenchmarkParamSpec& spec : specs) {

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

			// Left Panel - Benchmark Selection
			if (g_leftPanelOpen) {
				draw_leftPanel();
				ImGui::SameLine();
			}

			style::withChildWrapper("Main Content", ImVec2(0, 0), []() {
				// Top Bar - Benchmark Details + Parameters
				if (g_topBarOpen) draw_topBar();

				ImGui::Spacing();

				// Main Content - Benchmark Results + Hover Details
				draw_mainPage();

			}, false);
        });
    };

	// Left Panel - For Benchmark Selection
	void draw_leftPanel() {
		// ImVec2 _sidebarStartPos = ImGui::GetCursorScreenPos();

		style::withChildWrapper("Side Panel", ImVec2(sidePanelWidth, 0), []() {

			ImGui::Spacing();

			style::draw_textCentered("Benchmarks");

			ImGui::SameLine();

			float _arrowButtonSize = ImGui::GetFrameHeight();
			float avialableWidth = ImGui::GetContentRegionAvail().x;
			float _spacing = ImGui::GetStyle().ItemSpacing.x;

			ImGui::Dummy(ImVec2(avialableWidth - _arrowButtonSize - _spacing, 0.0f));
			ImGui::SameLine();

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
		// ImVec2 _topbarStartPos = ImGui::GetCursorScreenPos();

		// === Top Bar Layout ===
		style::withChildWrapper("Top Bar", ImVec2(0, topBarHeight), []() {

			// === Left Section of Top Bar [Benchmark Details] ===
			style::withChildWrapper("Benchmark Details", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0), []() {

				nostalgia::BenchmarkID _benchmarkID = nostalgia::benchmarking::loader::getBenchmarkID();

				if (_benchmarkID == nostalgia::BenchmarkID::NONE) {
					if (!g_leftPanelOpen) {
						if (ImGui::ArrowButton("##toggleSidePanel", g_leftPanelOpen ? ImGuiDir_Left : ImGuiDir_Right)) {
							g_leftPanelOpen = !g_leftPanelOpen;
						}
					}
					else {
						ImGui::Spacing();
						float _arrowButtonSize = ImGui::GetFrameHeight();
						ImGui::Dummy(ImVec2(_arrowButtonSize, _arrowButtonSize));
					}
					ImGui::SameLine();

					style::draw_textCentered("Select a Benchmark on the Left to get started!");
					//ImVec2
					//ImGui::Text("Select a Benchmark on the Left to get started!");
				}
				else {
					const nostalgia::BenchmarkType& benchmarkType = nostalgia::benchmark::atlas.at(_benchmarkID);

					if (!g_leftPanelOpen) {
						if (ImGui::ArrowButton("##toggleSidePanel", g_leftPanelOpen ? ImGuiDir_Left : ImGuiDir_Right)) {
							g_leftPanelOpen = !g_leftPanelOpen;
						}
					}
					else {
						ImGui::Spacing();
						float _arrowButtonSize = ImGui::GetFrameHeight();
						ImGui::Dummy(ImVec2(_arrowButtonSize, _arrowButtonSize));
					}
					ImGui::SameLine();
					style::draw_textCentered(benchmarkType.labelLong.c_str());

					//ImGui::Text("%s", benchmarkType.labelLong.c_str());
					ImGui::Spacing();
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::TextWrapped("%s", benchmarkType.description.c_str());
				}

				}, false);

			ImGui::SameLine();
			if (m_paramSpecs && m_parameters) { // If parameters are loaded
				// === Mid Section of Top Bar [Parameters] ===
				style::withChildWrapper("Benchmark Parameters", ImVec2(0, 0), []() {


				}, false);
			}
		});
    }

	void draw_mainPage_New() {
		if (m_paramSpecs && m_parameters) { 

			// === Benchmark Controls (Parameters and Launch Buttons) ===
			style::withChildWrapper("Benchmark Controls", ImVec2(0, 0), []() {
				ImVec2 _availSpace = ImGui::GetContentRegionAvail();

				// === Benchmark Specific Parameters ===
				style::withChildWrapper("Benchmark Parameters", ImVec2(_availSpace.x * 0.75f, _availSpace.y * 0.5f), []() {
					style::draw_textCentered("Benchmark Parameters");

					for (const BenchmarkParamSpec& spec : *m_paramSpecs) {
						const std::string& key = spec.key;
						const std::string& type = spec.typeName;

						ImGui::Text("%s", spec.description.c_str());
						ImGui::PushID(key.c_str());

						if (type == "int") {
							int& value = intInputs[key];
							if (ImGui::InputInt("##hiddenIntInput", &value, 1000, 10000))
								m_parameters->set<int>(key, value);
						}
						else if (type == "string") {
							std::string& buf = stringInputs[key];
							if (buf.empty() && spec.defaultValue) buf = *spec.defaultValue;

							char input[256];
							snprintf(input, sizeof(input), "%s", buf.c_str());
							//strncpy(input, buf.c_str(), sizeof(input));
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
						
						if (ImGui::IsItemHovered()) {
							m_hoveredParameter = &spec;
							m_hoverState = HoverDetailState::parameter;
						}
						ImGui::PopID();
					}
				}, false);

				ImGui::SameLine();

				// === Benchmark Specific Parameters ===
				style::withChildWrapper("Benchmark Buttons", ImVec2(0, _availSpace.y * 0.5f), []() {
					ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y * 0.8f));
					// === Run Benchmark Button ===
					ImVec2 _runButtonSize = ImVec2(ImGui::GetContentRegionAvail());

					if (ImGui::Button("Run Benchmark", _runButtonSize)) {
						nostalgia::benchmarking::loader::dispatchBenchmark(nostalgia::benchmarking::loader::getBenchmarkID());
						nostalgia::benchmarking::exporting::exportCurrentBenchmarks();
						nostalgia::visualiser::loadBenchmarkPlotData();
					}

					if (ImGui::IsItemHovered()) {
						m_hoverState = HoverDetailState::run;
					}

				}, false);

				// === Allocator Selection Flags ===
				style::draw_separatorSpace();

				style::withChildWrapper("Allocator Selection", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0), []() {

					style::draw_textCentered("Allocator/s Selection");

					ImGui::Spacing();

					// ~~~ Display ALL allocators ~~~
					for (const auto& [aID, aType] : nostalgia::allocator::atlas) {
						nostalgia::AllocatorFlags _benchmarkAllocatorFlags = nostalgia::benchmarking::loader::getAllocatorFlags();

						// ~~~ Display but Disable incompatible allocators ~~~
						bool _isCompatible = aType.isCompatibleWith(_benchmarkAllocatorFlags);
						bool _isIncluded = nostalgia::benchmarking::loader::isAllocatorInBenchmark(aID);

						if (!_isCompatible) ImGui::BeginDisabled();
						std::string _buttonLabel = (!_isCompatible ? "Incompatible" : (_isIncluded ? "Remove" : "Add"));

						ImGui::PushID(aType.label.c_str());
						if (_isIncluded) ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));
						
						// ~~~ Toggle Inclusion in Benchmark ~~~
						if (ImGui::Button(_buttonLabel.c_str(), allocatorButtonSize)) {
							nostalgia::benchmarking::loader::addAllocatorToBenchmark(aID, !_isIncluded);
						}

						if (ImGui::IsItemHovered()) {
							m_hoveredAllocator = aID;
							m_hoverState = HoverDetailState::allocator;
						}

						if (_isIncluded) ImGui::PopStyleColor();
						ImGui::PopID();

						if (!_isCompatible) ImGui::EndDisabled();

						ImGui::SameLine();

						std::string _allocatorLabel = std::format("{} [{}]", aType.label, aType.description);
						ImGui::TextWrapped("%s", _allocatorLabel.c_str());

						if (ImGui::IsItemHovered()) {
							m_hoveredAllocator = aID;
							m_hoverState = HoverDetailState::allocator;
						}

					}

				}, false);

				ImGui::SameLine();

				// === Implementation Selection Flags ===
				style::withChildWrapper("Implementation Selection", ImVec2(0, 0), []() {

					style::draw_textCentered("Implementation/s Selection");

					ImGui::Spacing();

					ImGui::TextWrapped("Implementations run PER Allocator, and automatically filter for compatability.\nUse this window to disable unwanted implementations across ALL allocators.");

					ImGui::Spacing();

					// ~~~ Display ALL implementations ~~~
					for (const auto& [iID, iType] : nostalgia::implementation::atlas) {						
						bool _isIncluded = nostalgia::benchmarking::loader::isImplementationInBenchmark(iID);

						std::string _buttonLabel = (_isIncluded ? "Disable" : "Enable");

						ImGui::PushID(iType.label);
						if (_isIncluded) ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonActive));

						// ~~~ Toggle Inclusion in Benchmark ~~~
						if (ImGui::Button(_buttonLabel.c_str(), allocatorButtonSize))
							nostalgia::benchmarking::loader::addImplementationToBenchmark(iID, !_isIncluded);

						if (ImGui::IsItemHovered()) {
							m_hoveredImplementation = iID;
							m_hoverState = HoverDetailState::implementation;
						}

						if (_isIncluded) ImGui::PopStyleColor();
						ImGui::PopID();

						ImGui::SameLine();

						std::string _implementationLabel = std::format("{}", iType.desc);
						ImGui::TextWrapped("%s", _implementationLabel.c_str());

						if (ImGui::IsItemHovered()) {
							m_hoveredImplementation = iID;
							m_hoverState = HoverDetailState::implementation;
						}
					}

					}, false);

			}, false);
		}
	}

	void draw_mainPage_HoverDetails() {
		switch (m_hoverState) {
		case HoverDetailState::NONE:
			break;
		case HoverDetailState::allocator: {
			std::string _allocatorLabel = nostalgia::allocator::atlas.at(m_hoveredAllocator).label;
			ImGui::Text("Allocator: %s", _allocatorLabel.c_str());
			break;
		}
		case HoverDetailState::implementation: {
			std::string _implementationLabel = nostalgia::implementation::atlas.at(m_hoveredImplementation).desc;
			ImGui::TextWrapped("Implementation: %s", _implementationLabel.c_str());
			break;
		}
		case HoverDetailState::parameter: {
			std::string _parameterLabel = m_hoveredParameter->key;
			ImGui::TextWrapped("Parameter: %s", _parameterLabel.c_str());
			std::string _parameterDescription = m_hoveredParameter->description;
			ImGui::TextWrapped("%s", _parameterDescription.c_str());
			break;
		}
		case HoverDetailState::run: {
			std::string _runLabel = std::format("This will launch the {} benchmark with your current parameters on your local machine.\n\nThis could take seconds or several minutes depending upon your settings.\n\nAre you ready?",
				nostalgia::benchmark::atlas.at(nostalgia::benchmarking::loader::getBenchmarkID()).label);
			ImGui::TextWrapped("%s", _runLabel.c_str());
		}
		default:
			break;
		}
	}

	// === Main Window [Parameters & Results] ===
    void draw_mainPage() {

		// ImVec2 _mainStartPos = ImGui::GetCursorScreenPos();
		style::withChildWrapper("Main Page", ImVec2(0, 0), []() {

			float _availWidth = ImGui::GetContentRegionAvail().x;
			float _panelWidth = std::max(_availWidth - mainPageRightPanelWidth, _availWidth * 0.66f);
			ImVec2 _tabSize = ImVec2(_panelWidth * 0.1f, 30.0f);

			// === Tabs ===
			ImGui::Dummy(ImVec2(20.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			ImGui::SameLine();
			if (style::draw_tabButton("New", (m_currentResultTab == -1), _tabSize)) m_currentResultTab = -1;
			ImGui::SameLine();
			ImGui::BeginDisabled();
			if (style::draw_tabButton("Results ", (m_currentResultTab == 1), _tabSize)) m_currentResultTab = 1;
			ImGui::EndDisabled();
			ImGui::PopStyleVar();

			// === Main Window ===
			style::withChildWrapper("Main Page Content", ImVec2(_panelWidth, 0), []() {
				
				if (m_currentResultTab == -1) draw_mainPage_New();
				else visualiser::ShowBenchmarkPlot();
			});

			ImGui::SameLine();

			// === Mouse Over Panel ===
			style::withChildWrapper("Main Page Details", ImVec2(0, 0), []() {

				style::draw_textCentered("(Mouse-over for more details here)");

				if (m_currentResultTab == -1) draw_mainPage_HoverDetails();
				else visualiser::ShowDetails();

			});
		}, false);
    }

	// Initialises SDL-Vulkan-DearIMGUI backend, calls draw_gui() every frame
	int init_gui() {
		init_renderBackend();
		return INIT_OK;
	};

}