#include "style.h"

namespace nostalgia::gui::style {
	bool draw_tabButton(const char* label, bool selected, ImVec2 size) {
		ImGui::PushID(label);
		if (!selected) ImGui::SetCursorPosY(ImGui::GetCursorPosY() + size.y * 0.1f); // Offset for selected tab
		bool pressed = ImGui::InvisibleButton("##tab", (selected ? ImVec2(size.x, size.y * 1.1f) : size));

		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// ImVec4 b = ImGui::GetStyle().Colors[ImGuiCol_Button];
		// ImVec4 h = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
		// ImVec4 a = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];

		ImU32 bgColor = ImGui::GetColorU32(
			selected ? ImGuiCol_ButtonHovered :
			ImGui::IsItemHovered() ? ImGuiCol_TabHovered :
			ImGuiCol_Button
		);

		drawList->AddRectFilled(min, max, bgColor, ImGui::GetStyle().TabRounding, ImDrawFlags_RoundCornersTop);

		ImVec2 textSize = ImGui::CalcTextSize(label);
		ImVec2 textPos = ImVec2(
			min.x + (size.x - textSize.x) * 0.5f,
			min.y + (size.y - textSize.y) * 0.5f
		);
		drawList->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), label);

		ImGui::PopID();
		return pressed;
	}

	void draw_textCentered(const char* text) {
		ImGui::Dummy(ImVec2(
			(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(text).x) * 0.5f - ImGui::GetStyle().ItemSpacing.x, 0));
		ImGui::SameLine();
		ImGui::Text("%s", text);
	}

	void draw_separatorSpace() {
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}

	void draw_verticalSeparator(float fillPercent) {
		ImGui::SameLine(0.0f, 0.0f);

		ImU32 color = ImGui::GetColorU32(ImGuiCol_Separator);
		float availHeight = ImGui::GetContentRegionAvail().y;
		float height = availHeight * (fillPercent > 0 ? fillPercent : 1.0f);
		float thickness = 1;

		ImVec2 screenPos = ImGui::GetCursorScreenPos();
		ImVec2 pos = ImVec2(
			screenPos.x,
			screenPos.y + (availHeight - height) * 0.5f
		);

		ImGui::GetWindowDrawList()->AddRectFilled(
			pos,
			ImVec2(pos.x + thickness, pos.y + height),
			color
		);
		ImGui::Dummy(ImVec2(thickness, height));
		ImGui::SameLine(0.0f, 0.0f);
	}
}