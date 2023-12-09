#pragma once

#include <iostream>
#include <time.h>
#include <ctime>
#include <unordered_map>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_viewer.h"
#include "database.h"
#include "imguidatechooser.h"

struct r_tm {
  tm _tm = {};
  bool changed = false;
  std::string* str = nullptr;
};

class Finances {
public:
  Finances();
  ~Finances();

public:
  void Render();

  void Summary();

  void PartialSummaries();

private:
  static int relation_chosen;

private:
  std::vector<std::string> names;
  r_tm date_from;
  r_tm date_to;
  void Calendar(int ID, r_tm& date);
  void DateToStr(r_tm& date);
  void DateDirection();
};