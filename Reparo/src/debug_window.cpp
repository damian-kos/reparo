// debug_window.cpp

#include "imgui.h"
#include <iostream>
static char text[1024 * 16] = "A";

void DebugWindow() {
	ImGui::Begin("Debug Window");
	ImGui::InputTextMultiline("##debuig_window", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_ReadOnly);
	ImGui::End();
}

void SetDebugWindowText(const char* newText) {
	strncpy_s(text, newText, IM_ARRAYSIZE(text));
}