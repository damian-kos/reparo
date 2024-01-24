#pragma once

#include "imgui.h"
#include "config.h"
#include "updates.h"
#include "modals.h"

class RO_Settings : public RO_Cfg {
public:
  static void Menu();

  static void FinancesWin();

  static void CheckUpdate();

private:

};