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
    RO_Settings::FinancesWin();
    RO_Settings::CheckUpdate();
    ImGui::EndMenu();
  }

}

void RO_Settings::FinancesWin() {
  static bool table_enabled = data["finances"]["tables"].contains("##repairs") ? data["finances"]["tables"]["##repairs"].get<bool>():true;
  if (ImGui::MenuItem("Enabled", "", &table_enabled)) {
    data["finances"]["tables"]["##repairs"] = table_enabled;
    RO_Cfg::UpdateCreateConfig(data);
  }
}

void RO_Settings::CheckUpdate() {
  //static bool table_enabled = data["finances"]["tables"].contains("##repairs") ? data["finances"]["tables"]["##repairs"].get<bool>() : true;
  if (ImGui::MenuItem("Check for Update")) {
    //std::string version_data = GetDetailsFromAPI(0);
    //std::cout << "Data: " << version_data << std::endl;
      //ModalController::RenderModal("Test");

    //if (REPARO_VERSION == version_data) {
    //  printf("Version is the same\n");
    //}
    //else if (version_data == "") {
    //  std::cout << " Can't parse" << std::endl;
    //}
    //else {
    //  std::cout << "Can be updated" << std::endl;

    //}
  }
}
