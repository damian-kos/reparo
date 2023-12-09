#pragma once
#include "finances.h"

int Finances::relation_chosen = 1;

Finances::Finances() {
  printf("Finances Created");
  //names = Database::GetRepairStatesNames();
  ImGui::SetDateToday(&date_from._tm);
  DateToStr(date_from);
  all_repairs_by_date = Database::RetreiveRepairsByDate(date_from.str, relation_chosen);
  repairs_to_table = &all_repairs_by_date;
  RefreshRepairs();
}
Finances::~Finances() { printf("Finances Destroyed"); }

void Finances::Render() {
  //printf("Total %.2f\n\n", all_repairs_by_date.total);

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
    all_repairs_by_date = Database::RetreiveRepairsByDate(date_from.str, relation_chosen);
    RefreshRepairs();
    date_from.changed = false;
  }
  if (date_to.changed) {
    if (date_from.str) {
      all_repairs_by_date = Database::RetreiveRepairsByDate(date_from.str, 3, date_to.str);
      RefreshRepairs();
    }

    date_to.changed = false;
  }

  ImGui::Spacing();
  ImGui::Spacing();
  Summary();
  PartialSummaries();

  RepairsView::RepairsToTable(*repairs_to_table, selected_state, false);
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
  std::string total = std::format("Total £ \n{0:.2f}", all_repairs_by_date.total);
  ImGui::ButtonScaled(total.c_str(), 2.0f, ImVec2(200, 215));
  ImGui::EndGroup();
  ImGui::SameLine();
}

void Finances::PartialSummaries() {
  int names_count = names.size() / 5 + ((names.size() % 5 != 0) ? 1 : 0);
  int j = 0; // Initialize j outside the loop

  for (int i = 0; i < names.size(); ++i) {
    if ((i % 5) == 0) {
      ImGui::BeginGroup();
      ++j; // Increment j every time i is divisible by 5 without remainder
    }
    std::string part_of_total = std::format("\n£{0:.2f}", repairs_per_state[i].total);
    std::string some_money = names[i] + part_of_total;
    if (ImGui::CenteredButton(names[i].c_str(), ImVec2(150, 40), some_money.c_str())) {
      Database::RetreiveRepairsByDate(date_from.str, relation_chosen, date_to.str, (i + 1));
      repairs_to_table = &repairs_per_state[i];
      selected_state = i;
    }

    if ((i % 5) == 4 || i == names.size() - 1) {
      ImGui::EndGroup();
      if (i != names.size() - 1) {
        ImGui::SameLine();
      }
    }
  }
}

void Finances::LeadingZero(int& date_int, std::string& date_str) {
  if (date_int >= 10) {
    date_str = std::to_string(date_int);
  }
  else {
    date_str = "0" + std::to_string(date_int);
  }
}

void Finances::DateToStr(r_tm& date) {
  date.str = new std::string;
  std::string year = std::to_string(date._tm.tm_year + 1900);
  std::string month;
  int month_int = date._tm.tm_mon + 1;
  LeadingZero(month_int, month);
  std::string day;
  LeadingZero(date._tm.tm_mday, day);

  *date.str = year + "-" + month + "-" + day;
}

void Finances::DateDirection() {
  static std::unordered_map<int, std::string> date_relation = {
    {1, "Before"},
    {2, "After"},
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

void Finances::RefreshRepairs() {
  repairs_per_state.clear();
  for (int i = 0; i < names.size(); i++)
  {
    RepairsSort new_repairs;
    new_repairs = Database::RetreiveRepairsByDate(date_from.str, relation_chosen, date_to.str, i + 1);
    repairs_per_state.emplace_back(new_repairs);
  }
}


