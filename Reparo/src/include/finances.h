#pragma once

#include <iostream>
#include <time.h>
#include <ctime>
#include <unordered_map>
#include <format> 

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_viewer.h"
#include "repairs.h"
#include "config.h"
#include "settings.h"


class FinancesWin : public RepairsView {
public:
  FinancesWin();
  ~FinancesWin();

public:
  void Render() override;
  std::shared_ptr<EditRepair> GetEditRepair() override;

private:
  int fi_table_select = -1;
  date_rel fi_relation;
  r_tm fi_date_from;
  r_tm fi_date_to;
  RepairsSort fi_all_repair_by_query;

private:
  RepairsSort* repairs_to_table;
  int selected_state = 0;
  std::vector<RepairsSort> repairs_per_state;
// IObserver / ISubject
private:
  void Update(const int& passed_int) override;
private:
  void Summary();
  void PartialSummaries();
  void RefreshRepairs() override;

private:
  //json settings_data;
};