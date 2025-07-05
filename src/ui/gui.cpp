#include "gui.h"

#include "render.h"
#include "style.h"

#include "allocators/allocator_meta.h"
#include "allocators/linear_bump/allocator_linear.h"

#include "benchmarking/benchmarking_manager.h"
#include "benchmarking/exporting/benchmarking_exporting.h"

#include "benchmarking/visualiser/benchmark_visualiser.h"


#include <iostream>

#include "imgui.h"

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

			for (const auto& button : benchmarking::getAllBenchmarks()) {
				if (button.disabled) ImGui::BeginDisabled();

				style::drawWideButton(button.label.c_str(), 0.9f, sidePanelButtonHeight, button.dispatcher);

				if (first) {
					first = false;
					ImGui::Spacing();
					ImGui::Separator();
				}
				if (button.disabled) ImGui::EndDisabled();
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
		
		style::withChildWrapper("Top Bar", ImVec2(0, topBarHeight), []() {

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