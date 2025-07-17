#include "gui.h"

#include "ui/system/renderer_vulkan.h"
#include "ui/system/style.h"

#include "allocators/info/allocator_meta.h"
#include "allocators/info/allocator_atlas.h"
#include "allocators/concrete/linear_bump/allocator_linear.h"

// #include "benchmarking/benchmarking_manager.h"
#include "benchmarking/exporter/benchmarking_exporting.h"
#include "benchmarking/info/benchmark_atlas.h"
#include "benchmarking/visualiser/benchmark_visualiser.h"
#include "benchmarking/dispatcher/benchmark_loader.h"
#include "benchmarking/info/benchmark_params.h"

#include "implementations/info/implementation_atlas.h"

#include "objects/info/object_meta.h"

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include "imgui.h"

#include "utils/log.h"
#include "utils/return_codes.h"

namespace nostalgia::gui{
    namespace {
		constexpr int 		new_benchmark_tab_index 				= -1;
		constexpr float 	browser_panel_max_width 				= 300.0f; 
		constexpr float 	browser_panel_min_width_perc	 		= 0.2f; 
		constexpr float 	browser_button_max_height 				= 50.0f;
		constexpr float 	header_bar_max_height 					= 300.0f; 
		constexpr float 	header_bar_min_height_perc 				= 0.3f; 
		constexpr float 	hover_panel_max_width 					= 500.0f;
		constexpr float 	hover_panel_min_width_perc 				= 0.3f;
		constexpr float 	new_benchmark_param_box_height_perc 	= 0.4f; 
		constexpr float 	new_benchmark_button_box_width_perc 	= 0.3f;
		constexpr float		new_benchmark_allocator_box_width_perc 	= 0.5f;
		constexpr ImVec2    tab_button_max_size						= ImVec2(100.0f, 30.0f);
		constexpr float 	tab_offset_x_perc						= 0.25f;
		constexpr ImVec2 	allocator_button_size					= ImVec2(120.0f, 0.0f);
		constexpr ImVec2 	expandable_size							= ImVec2(0.0f, 0.0f);
		constexpr float 	expandable_value						= 0.0f;
		constexpr int		input_int_button_step_size 				= 1000;
		constexpr int		input_int_button_alt_step_size 			= 10000;
		constexpr float 	launch_button_max_height 				= 60.0f;
		constexpr float 	launch_button_min_height_perc 			= 0.3f;

		bool 				browser_panel_open 						= true;
		bool 				header_bar_open 						= true;
		int 				current_result_tab 						= new_benchmark_tab_index; 

		// Input Parameter Maps
		const std::vector<BenchmarkParamSpec>* 				current_param_specs 		= nullptr; 
		nostalgia::BenchmarkParams* 						current_parameters		 	= nullptr;
		std::unordered_map<std::string, std::string> 		param_input_strings;
		std::unordered_map<std::string, int> 				param_input_ints;
		std::unordered_map<std::string, int> 				param_input_objects;
		std::unordered_map<std::string, bool> 				param_input_bools;

		// Hover Details
		enum class HoverDetailState {
			NONE,
			Allocator,
			Implementation,
			Parameter,
			Run
		};
		HoverDetailState 									hovered_state 				= HoverDetailState::NONE;
		nostalgia::ImplementationID 						hovered_implementation 		= nostalgia::ImplementationID::NONE;
		nostalgia::AllocatorID 								hovered_allocator 			= nostalgia::AllocatorID::Linear;
		const nostalgia::BenchmarkParamSpec* 				hovered_parameter 			= nullptr;
    }

	// Main GUI Entry Point Per Frame
    void draw_gui() {
		style::with_expanded_window_wrapper("Nostalgia", []() {

			if (browser_panel_open) {
				draw_browser_panel();
				ImGui::SameLine();
			}

			style::with_child_wrapper("Main Content", expandable_size, []() {
				if (header_bar_open) draw_header_bar();
				ImGui::Spacing();
				draw_main_page();
			}, style::BorderStyle::NONE, style::SpacingStyle::NONE);
        });
    };

	// Browser Panel - Benchmark Selection
	void draw_browser_panel() {
		style::with_child_wrapper("Browser Panel", ImVec2(
			std::min(browser_panel_max_width, ImGui::GetContentRegionAvail().x * browser_panel_min_width_perc),
			 expandable_value), []() {

			float initial_cursor_y = ImGui::GetCursorPosY();
			style::draw_text_centered("Benchmarks");
			ImGui::SameLine();

			float imgui_arrow_button_size = ImGui::GetFrameHeight();
			float browser_available_width = ImGui::GetContentRegionAvail().x;
			float imgui_spacing_x = ImGui::GetStyle().ItemSpacing.x;

			ImGui::Dummy(ImVec2(browser_available_width - 
				(imgui_arrow_button_size + imgui_spacing_x), expandable_value));
			ImGui::SameLine();
			ImGui::SetCursorPosY(initial_cursor_y + 
				(ImGui::GetTextLineHeight() - imgui_arrow_button_size) * 0.5f);
			
			if (ImGui::ArrowButton("##toggleSidePanel", ImGuiDir_Left)) browser_panel_open = false;

			if (ImGui::IsItemHovered()) ImGui::SetTooltip("Hide Benchmark Selection Panel (Toggle)");
			ImGui::Separator();
			ImGui::Spacing();

			// ~~~ Benchmark Selection Buttons ~~~
			for (const auto& [bID, bType] : nostalgia::benchmark::atlas) {
				if (bType.disabled) ImGui::BeginDisabled();

				style::draw_wide_button(
					bType.label.c_str(), 
					0.9f, 
					browser_button_max_height, 
					[&bID](){nostalgia::benchmarking::loader::load_benchmark(bID);});

				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Load and Prepare %s Benchmark...", bType.label.c_str());
				if (bType.disabled) ImGui::EndDisabled();
				ImGui::Spacing();
			}
		});
	}

	// Header Bar - General Information and Guidance
    void draw_header_bar() {
		style::with_child_wrapper("Header Bar", ImVec2(expandable_value, std::min(header_bar_max_height, ImGui::GetContentRegionAvail().y * header_bar_min_height_perc)), []() {
			style::with_child_wrapper("General Info", 
				ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, expandable_value), []() {

				nostalgia::BenchmarkID benchmark_id = nostalgia::benchmarking::loader::get_benchmark_id();
				float imgui_arrow_button_size = ImGui::GetFrameHeight();
				float imgui_spacing_y = ImGui::GetStyle().ItemSpacing.y;
				
				// ~~~ Browser Toggle Button ~~~
				if (!browser_panel_open) {
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (ImGui::GetTextLineHeight() - imgui_arrow_button_size) * 0.5f);
					if (ImGui::ArrowButton("##toggleBrowserPanel", ImGuiDir_Right)) browser_panel_open = true;
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Show Benchmark Selection Panel (Toggle)");
				}
				else ImGui::Dummy(ImVec2(imgui_arrow_button_size, imgui_arrow_button_size - imgui_spacing_y));
				ImGui::SameLine();

				// ~~~ Get Started Prompt ~~~
				if (benchmark_id == nostalgia::BenchmarkID::NONE)
					style::draw_text_centered("Select a Benchmark on the Left to get started!");
				
				// === General Information Display ===
				else {
					const nostalgia::benchmark_type& benchmark_type = nostalgia::benchmark::atlas.at(benchmark_id);
					style::draw_text_centered(benchmark_type.label_long.c_str());
					ImGui::Separator();

					// ~~~ Scrollable Benchmark Description ~~~
					style::with_child_wrapper("Benchmark Description", expandable_size, [&benchmark_type](){
						ImGui::TextWrapped("%s", benchmark_type.description.c_str());
					}, style::BorderStyle::NONE, style::SpacingStyle::NONE);
				}
			}, style::BorderStyle::NONE, style::SpacingStyle::SINGLE);

			style::draw_vertical_separator(0.9f);
			// === Code Preview === Replace with formatted code preview of implementation 
			if (current_param_specs && current_parameters) {
				style::with_child_wrapper("Code Preview", expandable_size, []() {
					// ~~~ [To-Do] Render Code Preview ~~~
				}, style::BorderStyle::NONE, style::SpacingStyle::SINGLE);
			}
		}, style::BorderStyle::LINE, style::SpacingStyle::NONE);
    }

	// === Main Page [Parameters & Results] ===
    void draw_main_page() {
		style::with_child_wrapper("Main Page", expandable_size, []() {
			float main_page_available_width = ImGui::GetContentRegionAvail().x;
			float main_page_usable_width = main_page_available_width - 
				std::min(hover_panel_max_width, hover_panel_min_width_perc * main_page_available_width);

			// ~~~ Tabs ~~~
			ImGui::Dummy(ImVec2(tab_button_max_size.x * tab_offset_x_perc, expandable_value));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, expandable_size);
			ImGui::SameLine();
			if (style::draw_tab_button("New", (current_result_tab == new_benchmark_tab_index), tab_button_max_size)) 
				current_result_tab = new_benchmark_tab_index;
			//ImGui::SameLine();

			size_t total_displayed_results = visualiser::get_total_displayed_benchmark_results();

			if (total_displayed_results > 0) {
				for (size_t i = 0; i < total_displayed_results; ++i) {
					ImGui::PushID(static_cast<int>(i));
					ImGui::SameLine();
					std::string label = visualiser::get_benchmark_label(i);
					if (style::draw_tab_button(label.c_str(), (current_result_tab == static_cast<int>(i)), tab_button_max_size)) 
						{
							visualiser::prepare_result_tab(i);
							current_result_tab = static_cast<int>(i);
						}
					ImGui::PopID();
				}
			}
			else{
				ImGui::SameLine();
				ImGui::BeginDisabled();
				style::draw_tab_button("Results ", false, tab_button_max_size);
				ImGui::EndDisabled();
			}

			ImGui::PopStyleVar();

			// === Main Input and/or Results Page ===
			style::with_child_wrapper("Main Page Content", ImVec2(main_page_usable_width, expandable_value), []() {
				if (current_result_tab == new_benchmark_tab_index) draw_setup_new_benchmark();
				else visualiser::draw_benchmark_results_view(static_cast<size_t>(current_result_tab));
			}, style::BorderStyle::LINE, style::SpacingStyle::NONE);
			ImGui::SameLine();

			// === Hover Details Panel ===
			style::with_child_wrapper("Hover Details Panel", expandable_size, []() {
				// style::draw_text_centered("[Mouse-over data for more details here]");
				if (current_result_tab == new_benchmark_tab_index) draw_hover_details();
				else visualiser::draw_benchmark_results_hover();
			});
		}, style::BorderStyle::NONE, style::SpacingStyle::NONE);
    }

	// === Hover Details Display (For New Benchmark) ===
	void draw_hover_details() {
		switch (hovered_state) {
		case HoverDetailState::NONE:
			break;
		case HoverDetailState::Allocator: {
			std::string allocator_label = nostalgia::allocator::atlas.at(hovered_allocator).label;
			ImGui::Text("Allocator: %s", allocator_label.c_str());
			break;
		}
		case HoverDetailState::Implementation: {
			std::string implementation_label = nostalgia::implementation::atlas.at(hovered_implementation).desc;
			ImGui::TextWrapped("Implementation: %s", implementation_label.c_str());
			break;
		}
		case HoverDetailState::Parameter: {
			std::string parameter_label = hovered_parameter->key;
			ImGui::TextWrapped("Parameter: %s", parameter_label.c_str());
			std::string _parameterDescription = hovered_parameter->description;
			ImGui::TextWrapped("%s", _parameterDescription.c_str());
			break;
		}
		case HoverDetailState::Run: {
			std::string run_label = std::format("This will launch the {} benchmark with your current parameters on your local machine.\n\nThis could take seconds or several minutes depending upon your settings.\n\nAre you ready?",
				nostalgia::benchmark::atlas.at(nostalgia::benchmarking::loader::get_benchmark_id()).label);
			ImGui::TextWrapped("%s", run_label.c_str());
		}
		default:
			break;
		}
	}


	// === Benchmark Controls (Parameters and Launch Buttons) ===
	void draw_setup_new_benchmark() {
		if (current_param_specs && current_parameters) { 
			style::with_child_wrapper("Benchmark Controls", expandable_size, []() {
				ImVec2 new_benchmark_available_size = ImGui::GetContentRegionAvail();
				// === Benchmark Specific Parameter Input ===
				style::with_child_wrapper("Benchmark Parameter Input Window", ImVec2(
					new_benchmark_available_size.x * (1.0f - new_benchmark_button_box_width_perc), 
					new_benchmark_available_size.y * new_benchmark_param_box_height_perc), []() {
					ImGui::Spacing();
					style::draw_text_centered("Benchmark Parameters");
					ImGui::Separator();

					// ~~~ Scrollable Display of Parameters ~~~
					style::with_child_wrapper("Parameter Inputs", expandable_size, []() {
						for (const BenchmarkParamSpec& spec : *current_param_specs) {
							const std::string& key = spec.key;
							const std::string& type = spec.type;
							ImGui::Text("%s", spec.description.c_str());
							ImGui::PushID(key.c_str());

							if (type == "int") {
								int& value = param_input_ints[key];
								if (ImGui::InputInt("##hiddenIntInput", &value, input_int_button_step_size, input_int_button_alt_step_size))
									current_parameters->set<int>(key, value);
							}
							else if (type == "object") {
								int& selected_index = param_input_objects[key];

								if (ImGui::Combo("Object", &selected_index, nostalgia::object_id_names.data(), static_cast<int>(nostalgia::object_id_names.size()))) {
									current_parameters->set<int>(key, selected_index);
								}
							}
							else if (type == "string") {
								std::string& buf = param_input_strings[key];
								if (buf.empty() && spec.default_value) buf = *spec.default_value;

								char input[256];
								snprintf(input, sizeof(input), "%s", buf.c_str());
								input[sizeof(input) - 1] = 0;

								if (ImGui::InputText(key.c_str(), input, sizeof(input))) {
									buf = input;
								}

								current_parameters->set<std::string>(key, buf);
							}
							else if (type == "bool") {
								bool& value = param_input_bools[key];
								ImGui::Checkbox(key.c_str(), &value);
								current_parameters->set<bool>(key, value);
							}
							else {
								ImGui::TextColored(ImVec4(1, 0, 0, 1), "Unsupported type: %s", type.c_str());
							}
							
							if (ImGui::IsItemHovered()) {
								hovered_parameter = &spec;
								hovered_state = HoverDetailState::Parameter;
							}
							ImGui::PopID();
						}
					}, style::BorderStyle::NONE, style::SpacingStyle::SINGLE);
				}, style::BorderStyle::NONE, style::SpacingStyle::HALF);
				ImGui::SameLine();

				// === New Benchmark Launch Buttons ===
				style::with_child_wrapper("Benchmark Launch Buttons", ImVec2(expandable_value,
					new_benchmark_available_size.y * new_benchmark_param_box_height_perc), []() {
					style::with_child_wrapper("Inner Highlight Box", expandable_size, []() {
						float launch_button_available_height = ImGui::GetContentRegionAvail().y;

						ImGui::Dummy(ImVec2(expandable_value, launch_button_available_height - 
							std::min(launch_button_max_height, launch_button_available_height * launch_button_min_height_perc)));
						
						if (ImGui::Button("Run Benchmark", ImGui::GetContentRegionAvail())) {
							nostalgia::benchmarking::loader::dispatch_benchmark(nostalgia::benchmarking::loader::get_benchmark_id());
						}
						if (ImGui::IsItemHovered()) {
							hovered_state = HoverDetailState::Run;
							ImGui::SetTooltip("Are you sure?!");
						}
					}, style::BorderStyle::HIGHLIGHT, style::SpacingStyle::HALF);
				}, style::BorderStyle::NONE, style::SpacingStyle::HALF);
				
				ImGui::Spacing();

				// === Allocator Selection ===
				style::with_child_wrapper("Allocator Selection", 
					ImVec2(ImGui::GetContentRegionAvail().x * new_benchmark_allocator_box_width_perc, expandable_value), []() {
					style::draw_text_centered("Allocator/s Selection");
					ImGui::Separator();
					style::with_child_wrapper("Allocator List", expandable_size, []() {

						// ~~~ Scrollable Display of ALL allocators ~~~
						for (const auto& [a_id, a_Type] : nostalgia::allocator::atlas) {
							nostalgia::AllocatorFlags benchmark_allocator_flags = nostalgia::benchmarking::loader::get_benchmark_required_flags();
							bool allocator_is_compatible = a_Type.is_compatible_with_benchmark(benchmark_allocator_flags);
							bool allocator_is_included = nostalgia::benchmarking::loader::is_allocator_in_benchmark(a_id);

							if (!allocator_is_compatible) ImGui::BeginDisabled();
							std::string button_label = (!allocator_is_compatible ? "Incompatible" : 
								(allocator_is_included ? "Remove" : "Add"));

							ImGui::PushID(a_Type.label.c_str());
							if (allocator_is_included) ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonHovered));
							
							// ~~~ Toggle Inclusion in Benchmark ~~~
							if (ImGui::Button(button_label.c_str(), allocator_button_size)) {
								if(allocator_is_included) 
									nostalgia::benchmarking::loader::remove_allocator_from_benchmark(a_id);
								else 
									nostalgia::benchmarking::loader::add_allocator_to_benchmark(a_id);

							}

							if (ImGui::IsItemHovered()) {
								hovered_allocator = a_id;
								hovered_state = HoverDetailState::Allocator;
							}

							if (allocator_is_included) ImGui::PopStyleColor();
							ImGui::PopID();
							if (!allocator_is_compatible) ImGui::EndDisabled();
							ImGui::SameLine();

							std::string allocator_label = std::format("{} [{}]", a_Type.label, a_Type.description);
							ImGui::TextWrapped("%s", allocator_label.c_str());

							if (ImGui::IsItemHovered()) {
								hovered_allocator = a_id;
								hovered_state = HoverDetailState::Allocator;
							}
						}
					}, style::BorderStyle::NONE, style::SpacingStyle::NONE);
				}, style::BorderStyle::NONE, style::SpacingStyle::HALF);

				style::draw_vertical_separator(0.6f);

				// === Implementation Selection Flags ===
				style::with_child_wrapper("Implementation Selection", expandable_size, []() {
					style::draw_text_centered("Implementation/s Selection");
					ImGui::Separator();
					// ~~~ Scrollable Display ALL Implementations ~~~
					style::with_child_wrapper("Implementation List", expandable_size, []() {

						ImGui::TextWrapped("Implementations run PER Allocator, and automatically filter for compatability.\nUse this window to disable unwanted implementations across ALL allocators.");
						ImGui::Spacing();

						for (const auto& [i_id, i_Type] : nostalgia::implementation::atlas) {						
							bool imp_is_included = nostalgia::benchmarking::loader::is_implementation_in_benchmark(i_id);
							std::string button_label = (imp_is_included ? "Disable" : "Enable");
							ImGui::PushID(i_Type.label);
							if (imp_is_included) ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_ButtonHovered));

							// ~~~ Toggle Inclusion in Benchmark ~~~
							if (ImGui::Button(button_label.c_str(), allocator_button_size)){
								if(imp_is_included)
									nostalgia::benchmarking::loader::remove_implementation_from_benchmark(i_id);
								else
									nostalgia::benchmarking::loader::add_implementation_to_benchmark(i_id);
							}

							if (ImGui::IsItemHovered()) {
								hovered_implementation = i_id;
								hovered_state = HoverDetailState::Implementation;
							}

							if (imp_is_included) ImGui::PopStyleColor();
							ImGui::PopID();
							ImGui::SameLine();
							std::string imp_label = std::format("{}", i_Type.desc);
							ImGui::TextWrapped("%s", imp_label.c_str());

							if (ImGui::IsItemHovered()) {
								hovered_implementation = i_id;
								hovered_state = HoverDetailState::Implementation;
							}
						}
					}, style::BorderStyle::NONE, style::SpacingStyle::NONE);
				}, style::BorderStyle::NONE, style::SpacingStyle::HALF);

			}, style::BorderStyle::NONE, style::SpacingStyle::NONE);
		}
	}

	// Initialises SDL-Vulkan-DearImgui backend, which calls draw_gui() every frame
	ReturnCode init_gui() {
		return init_render_backend();
	};

	void load_benchmarking_params(const std::vector<BenchmarkParamSpec>& specs) {
		current_param_specs = &specs;
		current_parameters = &nostalgia::benchmarking::loader::get_parameters();

		for (const BenchmarkParamSpec& spec : specs) {
			if (spec.type == "int") {
				param_input_ints[spec.key] = spec.default_value ? std::stoi(*spec.default_value) : 0;
			} else if (spec.type == "object") {
				param_input_objects[spec.key] = spec.default_value ? std::stoi(*spec.default_value) : 0;
			}
			else if (spec.type == "string") {
				param_input_strings[spec.key] = spec.default_value ? *spec.default_value : "";
			} else if (spec.type == "bool") {
				param_input_bools[spec.key] = spec.default_value ? (*spec.default_value == "true") : false;
			}
		}
	}
}