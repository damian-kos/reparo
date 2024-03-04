#pragma once

#include "imgui.h"
#include "imgui_stdlib.h"
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

  static void TermsAndConditionsMenu();

  static void TCsSttgsWin();

public:
  struct FinancesWinOpts {
    static inline bool table_repairs;
  };

  struct TCsWinOpts {
    static inline int max_size = 3000;
    static inline int size_left = 0;

    static inline bool tcs_win_on = false;
    static inline std::string str = "";
  };
};