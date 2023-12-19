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
