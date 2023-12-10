#pragma once

#include <iostream>
#include <memory>

#include "imgui.h"
#include "edit_repair.h"

struct r_tm {
  tm _tm = {};
  bool changed = false;
  std::string* str = nullptr;
};

struct date_rel {
  int number = 1;
  bool changed = false;
};

class RepairsView {
public:
  RepairsView();

  ~RepairsView();

public:
  virtual void Render();
  void OnDatesChanged(r_tm& date_from, int& relation, r_tm& date_to,  RepairsSort& repairs, int state = 0);
  void OnDatesDirectionChange(r_tm& date_from, date_rel& relation, r_tm& date_to, RepairsSort& repairs, int state);
  void Calendar(int ID, r_tm& date);
  void DateToStr(r_tm& date);
  void DateDirection(date_rel& relation);
  void CalendarsRender(const int& relation, r_tm& date_from, r_tm& date_to);
  void RepairsToTable(r_tm& date_from, int& relation, r_tm& date_to, const int state, RepairsSort& repairs, int& selected);
  void RunModal();
  std::shared_ptr<EditRepair> GetEditRepair();
  virtual void RefreshRepairs();

private:
  void StatesTabBar();
  void LeadingZero(int& date_int, std::string& date_str);

protected:
  bool* opened;
  std::vector<std::string> names; // names of states in Db


private:
  int rv_table_select = -1;
  date_rel rv_relation;
  r_tm rv_date_from;
  r_tm rv_date_to;
  RepairsSort rv_all_repairs_by_query;

private:
  int prev_chosen_tab;
  int curr_chosen_tab;
  Repair repair_to_init;
  bool modal;
  int modal_on_id;
  ConfirmResult deletion = ConfirmResult::CONIFRM_IDLE;
  std::shared_ptr<EditRepair> edit_repair;
};