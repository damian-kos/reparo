#include "settings.h"

json data = RO_Cfg::GetConfig();

void RO_Settings::Menu(){
  if (ImGui::BeginMenu("Options"))
  {

    static bool enabled = true;
    ImGui::MenuItem("Enabled", "", &enabled);
  
    static int style_idx = 0;
    if (ImGui::Combo("Theme", &style_idx, "Dark\0Light\0Classic\0"))
    {
      switch (style_idx)
      {
      case 0: ImGui::StyleColorsDark(); break;
      case 1: ImGui::StyleColorsLight(); break;
      case 2: ImGui::StyleColorsClassic(); break;
      }
      data["settings"]["theme"] = style_idx;
      RO_Cfg::UpdateCreateConfig(data);
    }
    RO_Settings::FinancesWinWin();
    RO_Settings::CheckUpdate();
    RO_Settings::TicketPrintableTemplate();
    RO_Settings::UploadCompanyLogo();
    RO_Settings::TermsAndConditionsMenu();
    ImGui::EndMenu();
  }

}

void RO_Settings::FinancesWinWin() {
  //static bool table_enabled = data["finances"]["tables"].contains("##repairs") ? data["finances"]["tables"]["##repairs"].get<bool>() : true;
  //FinancesWinOpts::table_repairs = RO_Cfg::getValue("finances.tables.##repairs", true);
  if (ImGui::BeginMenu("Finances & Accounting"))
  {
    if (ImGui::MenuItem("View table with repairs", "", &FinancesWinOpts::table_repairs)) {
        data["finances"]["tables"]["##repairs"] = FinancesWinOpts::table_repairs;
        RO_Cfg::UpdateCreateConfig(data);
      }
  ImGui::EndMenu();
  }
}

void RO_Settings::CheckUpdate() {
  //static bool table_enabled = data["finances"]["tables"].contains("##repairs") ? data["finances"]["tables"]["##repairs"].get<bool>() : true;
  if (ImGui::MenuItem("Check for Update")) {
    Updater::available_version = Updater::GetDetailsFromAPI(0);
    std::cout << "Available version: " << Updater::available_version << std::endl;
    if (REPARO_VERSION == Updater::available_version) {
      // Modal saying version is up to date.
      std::cout << "Version is up to date" << std::endl;
      Updater::update_available = false;
      return;
    }
    else if (Updater::available_version == "") {
      // Modal informing about an error.
      Updater::update_available = false;

      return;
    }
    else {
      Updater::update_available = true;

    }
  }
}

void RO_Settings::TicketPrintableTemplate() {
  if (ImGui::MenuItem("Printable Template")) {
    RepairTicket::show_window = !RepairTicket::show_window;
  }
}

void RO_Settings::UploadCompanyLogo() {
  if (ImGui::MenuItem("Load Company Logo")) {
    IGFD::FileManager file_manager;
    file_manager.GetDrives();
    IGFD::FileDialogConfig config;
    config.path = ".";
    ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir, "", ImVec4(0.5f, 1.0f, 0.9f, 0.9f), "\xF0\x9F\x93\x81");
    ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".jpg", ImVec4(0.5f, 1.0f, 0.9f, 0.9f), "\xF0\x9F\x8C\x84");
    ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByExtention, ".png", ImVec4(0.5f, 1.0f, 0.9f, 0.9f), "\xF0\x9F\x8C\x84");
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png",config);
  }
}

void RO_Settings::DisplayLogoUploader() {
  // display
  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
    if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      std::cout << "File path name: " << filePathName << std::endl;

      std::cout << "File path: " << filePath << std::endl;
      Logo::MoveLogo(filePathName);
    }
    // close
    ImGuiFileDialog::Instance()->Close();
  }

}

void RO_Settings::TermsAndConditionsMenu() {
  if (ImGui::MenuItem("Terms & Conditions")) {
    TCsWinOpts::tcs_win_on = true;
    TCsWinOpts::str = RO_Cfg::getValue("text_field.Terms & Conditions.text", TCsWinOpts::str, "template.json");
    TCsWinOpts::size_left = TCsWinOpts::max_size - TCsWinOpts::str.size();
  }
}

void RO_Settings::TCsSttgsWin() {
  if (TCsWinOpts::tcs_win_on) {
    ImGui::Begin("Terms & Conditions", &TCsWinOpts::tcs_win_on);
   
    ImGui::InputTextMultiline("Terms & Conditions", &TCsWinOpts::str);
    if (ImGui::IsItemEdited()) {
      TCsWinOpts::size_left = TCsWinOpts::max_size - TCsWinOpts::str.size();
    }
    ImGui::Text("(%d)", TCsWinOpts::size_left);
    ImGui::BeginDisabled(TCsWinOpts::size_left < 0);
    if (ImGui::Button("Save T&C")) {
      UpdateCreateConfig("text_field.Terms & Conditions.text", TCsWinOpts::str, "template.json");
      std::cout << TCsWinOpts::str << std::endl;
    }
    ImGui::EndDisabled();

    ImGui::End();
  }
}
