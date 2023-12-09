#pragma once

#include <iostream>
#include <time.h>
#include <ctime>
#include <unordered_map>
#include <format> 

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_viewer.h"
#include "database.h"
#include "imguidatechooser.h"
#include "repairs.h"

struct r_tm {
  tm _tm = {};
  bool changed = false;
  std::string* str = nullptr;
};

class Finances : RepairsView {
public:
  Finances();
  ~Finances();

public:
  void Render() override;

private:
  static int relation_chosen;
  RepairsSort all_repairs_by_date;
  RepairsSort* repairs_to_table;
  int selected_state = 0;
  std::vector<RepairsSort> repairs_per_state;
  r_tm date_from;
  r_tm date_to;

private:
  void Calendar(int ID, r_tm& date);
  void Summary();
  void PartialSummaries();
  void LeadingZero(int& date_int, std::string& date_str);
  void DateToStr(r_tm& date);
  void DateDirection();
  void RefreshRepairs();
};