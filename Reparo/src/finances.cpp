#pragma once
#include "finances.h"

Finances::Finances() { printf("Finances Created"); }
Finances::~Finances() { printf("Finances Destroyed"); }

void Finances::Render() {
  ImGui::ButtonScaled("Last 30 days\n   £1000", 1.5f, ImVec2(200, 80));
  ImGui::SameLine();
  ImGui::ButtonScaled("Ongoing Repairs\n   £1000", 1.5f, ImVec2(200, 80));

  
}