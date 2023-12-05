#pragma once
#include "finances.h"

Finances::Finances() { printf("Finances Created"); }
Finances::~Finances() { printf("Finances Destroyed"); }

void Finances::Render() {
  ImGui::BeginGroup();
  if (ImGui::ButtonScaled("   £1000.68", 1.5f, ImVec2(200, 80))) {
    ImGui::OpenPopup("my popup");
  }

  ImGui::EndGroup();
  ImGui::SameLine();
  ImGui::BeginGroup();;
 
  ImGui::EndGroup();
  
  if (ImGui::BeginPopupContextItem("my popup"))
  {
    if (ImGui::Selectable("To do")) {
      std::cout << "To do" << std::endl;
      Database::RetreiveRepairsByDate("2023-11-16");
    }

    ImGui::EndPopup();
  }
}