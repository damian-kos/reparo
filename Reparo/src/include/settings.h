#pragma once

#include "imgui.h"
#include "config.h"
#include "updates.h"
#include "modals.h"
#include "repair_ticket.h"
#include "ImGuiFileDialog.h"



class RO_Settings : public RO_Cfg {
public:
  static void Menu();
  static void FinancesWinWin();
  static void CheckUpdate();

  static void TicketPrintableTemplate();

  static void UploadCompanyLogo();

  static void DisplayLogoUploader();

public:
  struct FinancesWinOpts {
    //static inline bool table_repairs = data["finances"]["tables"].contains("##repairs") ? data["finances"]["tables"]["##repairs"].get<bool>() : true;
    static inline bool table_repairs;

  };

};