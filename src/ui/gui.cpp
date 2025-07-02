#include "gui.h"

#include "render.h"
#include "style.h"

#include "allocators/allocator_meta.h"
#include "allocators/linear_bump/allocator_linear.h"

#include "benchmarking/benchmarking_manager.h"

#include <iostream>

#include "imgui.h"

#include "log.h"

namespace nostalgia::gui{

    namespace {
		bool g_leftPanelOpen = true;
		bool g_topBarOpen = true;

		float sidePanelWidth = 300.0f; 
		float sidePanelButtonHeight = 30.0f;

		float topBarHeight = 300.0; 

		float mainPageRightPanelWidth = 400.0f; // Width of the right panel on the main page
    }


    int init_gui () {
		log::print("Initializing GUI...");

        benchmarking::init_benchmarking_manager();

        init_render();

        return 0;
    };


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


	void draw_leftPanel() {
		//ImGuiIO& _io = ImGui::GetIO();
		ImVec2 _sidebarStartPos = ImGui::GetCursorScreenPos();
		//log::print("Drawing left panel at [{}, {}]", _sidebarStartPos.x, _sidebarStartPos.y);

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

				style::drawWideButton(button.label.c_str(), 0.9f, sidePanelButtonHeight, button.run);

				if (first) {
					first = false;
					ImGui::Spacing();
					ImGui::Separator();
				}
				if (button.disabled) ImGui::EndDisabled();
				ImGui::Spacing();
			}
		});
	}

    void draw_topBar() {
		ImVec2 _topbarStartPos = ImGui::GetCursorScreenPos();
		//log::print("Drawing top panel at [{}, {}]", _topbarStartPos.x, _topbarStartPos.y);
		
		style::withChildWrapper("Top Bar", ImVec2(0, topBarHeight), []() {

		});
    }

    void draw_mainPage() {
		ImVec2 _mainStartPos = ImGui::GetCursorScreenPos();
		//log::print("Drawing main panel at [{}, {}]", _mainStartPos.x, _mainStartPos.y);
		
		style::withChildWrapper("Main Page", ImVec2(0, 0), []() {
			
			float _availWidth = ImGui::GetContentRegionAvail().x;
			float _panelWidth = std::max(_availWidth - mainPageRightPanelWidth, _availWidth * 0.66f);
			style::withChildWrapper("Main Page Content", ImVec2(_panelWidth, 0), []() {

			ImGui::Text("Main Page");

			});

			ImGui::SameLine();

			style::withChildWrapper("Main Page Details", ImVec2(0, 0), []() {

				ImGui::Text("Main Page Details");

			});
		}, false);
    }
}