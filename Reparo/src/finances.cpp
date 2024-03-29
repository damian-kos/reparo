﻿#pragma once
#include "finances.h"

FinancesWin::FinancesWin() {
  printf("FinancesWin Created\n");
  //settings_data = RO_Settings::GetConfig("config.json");
  repairs_to_table = &fi_all_repair_by_query;
  RefreshRepairs();
  RO_Settings::FinancesWinOpts::table_repairs = RO_Cfg::getValue("finances.tables.##repairs", false, "config.json");
}
FinancesWin::~FinancesWin() { printf("FinancesWin Destroyed"); }

void FinancesWin::Render() {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Options"))  {
      RO_Settings::FinancesWinWin();
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  DateDirection(fi_relation); ImGui::SameLine();
  CalendarsRender(fi_relation.number, fi_date_from, fi_date_to);
  ImGui::SameLine();

  OnDatesChanged(fi_date_from, fi_relation.number, fi_date_to,
                 *repairs_to_table);
  OnDatesDirectionChange(fi_date_from, fi_relation, fi_date_to,
    *repairs_to_table);

  ImGui::Spacing();
  ImGui::Spacing();
  Summary();
  PartialSummaries();
  static int state_tmp = 0;
  if (ImGui::CollapsingHeader("Tables", &RO_Settings::FinancesWinOpts::table_repairs)) {
    if (repairs_to_table)
      RepairsView::RepairsToTable(fi_date_from, fi_relation.number, fi_date_to, state_tmp, *repairs_to_table, fi_table_select);
  }
  RepairsView::RunModal();
}


void FinancesWin::Summary() {
  ImGui::BeginGroup();
  std::string total = std::format("Total £ \n{0:.2f}", fi_all_repair_by_query.total);
  if (ImGui::ButtonScaled(total.c_str(), 2.0f, ImVec2(200, 215))) {
    repairs_to_table = &fi_all_repair_by_query;
  }
  ImGui::EndGroup();
  ImGui::SameLine();
}

void FinancesWin::PartialSummaries() {
  int names_count = static_cast<int>(names.size() / 5 + ((names.size() % 5 != 0) ? 1 : 0));
  int j = 0; // Initialize j outside the loop

  for (int i = 0; i < names.size(); i++) {
    if ((i % 5) == 0) {
      ImGui::BeginGroup();
      ++j; // Increment j every time i is divisible by 5 without remainder
    }
    std::string part_of_total = std::format("\n£{0:.2f}", repairs_per_state[i].total);
    std::string some_money = names[i] + part_of_total;
    if (ImGui::CenteredButton(names[i].c_str(), ImVec2(150, 40), some_money.c_str())) {
      //Database::RetreiveRepairsByDate(fi_date_from.str, fi_relation.number, fi_date_to.str, (i + 1));
      repairs_to_table = &repairs_per_state[i];
      selected_state = i+1;
     
    }

    if ((i % 5) == 4 || i == names.size() - 1) {
      ImGui::EndGroup();
      if (i != names.size() - 1) {
        ImGui::SameLine();
      }
    }
  }
}

void FinancesWin::RefreshRepairs() {
  repairs_per_state.clear();
  
  for (int i = 0; i <= names.size(); i++)
  {
    RepairsSort new_repairs;
    new_repairs = Database::RetreiveRepairsByDate(fi_date_from.str, fi_relation.number, fi_date_to.str, i);
    if (i == 0)
      fi_all_repair_by_query = new_repairs;
    else
      repairs_per_state.emplace_back(new_repairs);
    //printf("CURRENTLY I IS: %d\n", i);
  }
}

void FinancesWin::Update(const int& passed_int, Repair* repair) {
  printf("Do cool stuff on finances tables");
  RefreshRepairs();
}

std::shared_ptr<EditRepair> FinancesWin::GetEditRepair()
{
  return RepairsView::GetEditRepair();
}


