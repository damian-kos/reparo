#include "imgui_decorator.h"

ImGuiDecorator::ImGuiDecorator() {}

void ImGuiDecorator::DecorateSeparatorText(const char* text) {
    if (decorate) {
        ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    ImGui::SeparatorText(text);

    ImGui::PopStyleColor(1);
}

void ImGuiDecorator::SetTestValue(bool new_value) {
    decorate = new_value;
}