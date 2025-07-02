#pragma once

#include "imgui.h"
#include <string_view>
#include <functional>

namespace nostalgia::gui::style {
	template <typename F>
	void withChildWrapper(const char* label, ImVec2 size, F&& fn, bool border = true) {
		if (!border) ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
		ImGui::BeginChild(label, size, border);
		if (!border) ImGui::PopStyleVar();

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

		//ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * (1 - widthOfAvail) * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);

		ImGui::SetCursorPosX(cursorX + (availableWidth - buttonWidth) * 0.5f);

		if (ImGui::Button(label, ImVec2(buttonWidth, height))) {
			std::forward<F>(onClick)();
		}

		//	ImGui::PopID();
	}
}