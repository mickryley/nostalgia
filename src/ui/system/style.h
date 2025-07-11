#pragma once

#include "imgui.h"
#include <string_view>
#include <functional>

namespace nostalgia::gui::style {

	enum class BorderStyle{
		NONE,
		LINE,
		HIGHLIGHT
	};

	enum class SpacingStyle{
		NONE,
		HALF,
		SINGLE,
		DOUBLE
	};


	template <typename F>
	void withChildWrapper(const char* label, ImVec2 size, F&& fn, 
		BorderStyle border = BorderStyle::LINE, SpacingStyle spacing = SpacingStyle::SINGLE) {
		
		//ImVec2 _curPadding = ImGui::GetStyle().WindowPadding;

		switch(spacing) {
			case SpacingStyle::HALF:
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(7.0f, 7.0f));
				break;
			case SpacingStyle::SINGLE:
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0f, 14.0f));
				break;
			case SpacingStyle::DOUBLE:
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(28.0f, 28.0f));
				break;
			case SpacingStyle::NONE:
				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				break;
		}

		switch (border){
			case BorderStyle::LINE:
				ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetColorU32(ImGuiCol_Border));
				break;
			case BorderStyle::HIGHLIGHT:
			ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetColorU32(ImGuiCol_Separator));
				break;
			case BorderStyle::NONE:
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0,0,0,0));
				break;
		}

		bool _drawBorder = (border != BorderStyle::NONE || spacing != SpacingStyle::NONE);
		ImGui::BeginChild(label, size, _drawBorder); // true = border
		
		ImGui::PopStyleColor(); // 1x Border Color
		ImGui::PopStyleVar(3); // 3x Child Spacing

		std::forward<F>(fn)();

		ImGui::EndChild();
	}

	template <typename F>
	void withExpandedWindow(std::string_view title, F&& fn) {

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);

		ImGui::Begin(title.data(), nullptr,
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoBringToFrontOnFocus);


		std::forward<F>(fn)();

		ImGui::End();
	}

	template <typename F>
	void drawWideButton(const char* label, float widthOfAvail, float height, F&& onClick) {
		//ImGui::PushID(label);
		float cursorX = ImGui::GetCursorPosX();
		float availableWidth = ImGui::GetContentRegionAvail().x;
		float buttonWidth = availableWidth * widthOfAvail;

		ImGui::SetCursorPosX(cursorX + (availableWidth - buttonWidth) * 0.5f);

		if (ImGui::Button(label, ImVec2(buttonWidth, height))) {
			std::forward<F>(onClick)();
		}
		//	ImGui::PopID();
	}

	bool draw_tabButton(const char* label, bool selected, ImVec2 size);

	void draw_textCentered(const char* text);

	void draw_separatorSpace();

	void draw_verticalSeparator(float fillPercent);
}