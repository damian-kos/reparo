#include "repairs_states.h"

int selected = -1;


void RepairMenu::RepairMainWin() {
    ImGui::Begin("Repairs");
    static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_AutoSelectNewTabs;
    const char* names[4] = { "To do", "Processing", "Warranty", "Awaiting Parts" };
    static bool opened[4] = { true, true, true, true };
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
            if (opened[n] && ImGui::BeginTabItem(names[n], &opened[n], ImGuiTabItemFlags_None))
            {
                repairs.RepairsSelector(n);
                ImGui::EndTabItem();
            }
        ImGui::EndTabBar();
    }

    
    ImGui::End();

}

