#pragma once
#include "finances.h"

Finances::Finances() { printf("Finances Created"); }
Finances::~Finances() { printf("Finances Destroyed"); }

void Finances::Render() {
  ImGui::BeginGroup();
  ImGui::Text("Last 30 days");
  ImGui::ButtonScaled("   £1000", 1.5f, ImVec2(200, 80));
  ImGui::EndGroup();
  ImGui::SameLine();
  ImGui::BeginGroup();
  ImGui::Text("Ongoing Repairs");
  ImGui::ButtonScaled("   £1000", 1.5f, ImVec2(200, 80));
  ImGui::EndGroup();
  
}