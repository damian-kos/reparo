#pragma once
#include "finances.h"

int Finances::relation_chosen = 1;

Finances::Finances() { 
  printf("Finances Created");
  names = Database::GetRepairStatesNames();
}
Finances::~Finances() { printf("Finances Destroyed"); }

void Finances::Render() {
  DateDirection();
  ImGui::SameLine();
  
  Calendar(1, date_from); ImGui::SameLine();  
  ImGui::BeginDisabled(!(relation_chosen == 3));
  Calendar(2, date_to);
  ImGui::EndDisabled();

  if (!(relation_chosen == 3)) {
    if (date_to.str) {
      delete date_to.str;
      date_to.str = nullptr;
    }
  }

  if (date_from.changed) {
    Database::RetreiveRepairsByDate(date_from.str, relation_chosen);
    date_from.changed = false;
  }
  if (date_to.changed) {
    if(date_from.str)
      Database::RetreiveRepairsByDate(date_from.str, 3, date_to.str);
    date_to.changed = false;
  }

  ImGui::Spacing();
  ImGui::Spacing();
  Summary();
  PartialSummaries();
}

void Finances::Calendar(int ID, r_tm& date) {

  ImGui::PushID(ID);
  ImGui::PushItemWidth(250);
  if (ImGui::DateChooser("##From", date._tm)) {
    DateToStr(date);
    date.changed = true;
  }
  ImGui::PopItemWidth();
  ImGui::PopID();
}

void Finances::Summary() {
  ImGui::BeginGroup();
  ImGui::ButtonScaled("To do:\n£1000.68", 2.0f, ImVec2(200, 215));
  ImGui::EndGroup();
  ImGui::SameLine();
  ImGui::BeginGroup();
}

void Finances::PartialSummaries()
{
  for (auto& pair : names) {
    std::string some_money = pair + "\n£214.58";
    if (ImGui::CenteredButton(pair.c_str(), ImVec2(150, 40), some_money.c_str())) {

    }
  }
  ImGui::EndGroup();
}


void Finances::DateToStr(r_tm& date) {
  date.str = new std::string;
  std::string year = std::to_string(date._tm.tm_year + 1900);
  std::string month = std::to_string(date._tm.tm_mon + 1);
  std::string day;
  if (date._tm.tm_mday >= 10) {
    day = std::to_string(date._tm.tm_mday);
  }
  else {
    day = "0" + std::to_string(date._tm.tm_mday);
  }
  *date.str = year + "-" + month + "-" + day;
}

void Finances::DateDirection() {
  static std::unordered_map<int, std::string> date_relation = {
    {1, "After"},
    {2, "Before"},
    {3, "Between"}
  };
  ImGui::PushItemWidth(100);
  if (ImGui::BeginCombo("##Relation", date_relation[relation_chosen].c_str())) {
    for (auto& pair : date_relation) {
      const bool is_selected = (relation_chosen == pair.first);
      if (ImGui::Selectable(date_relation[pair.first].c_str(), is_selected)) {
        relation_chosen = pair.first;
      }
    }
    ImGui::EndCombo();
  }
  ImGui::PopItemWidth();
}


  
