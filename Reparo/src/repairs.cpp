#include "repairs.h"

RepairsView::RepairsView() { 
    std::cout << "RepairsView Created empty" << std::endl;

    repairs = Database::RetreiveRepairsOfState(curr_chosen_tab+1);
}

RepairsView::~RepairsView() { std::cout << "RepairsView dESTROYED " << std::endl; }

void RepairsView::Render() {
  static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_AutoSelectNewTabs;
  const char* names[4] = { "To do", "Processing", "Warranty", "Awaiting Parts" };
  static bool opened[4] = { true, true, true, true };
    
  if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
  {
    for (int n = 0; n < IM_ARRAYSIZE(opened); n++) {
      if (opened[n] && ImGui::BeginTabItem(names[n], &opened[n], ImGuiTabItemFlags_None))
      {
        curr_chosen_tab = n;
        if (curr_chosen_tab != prev_chosen_tab) {
          std::cout << "Reload repairs for this tab: " << n+1 << std::endl;
          repairs = Database::RetreiveRepairsOfState(n + 1);
          prev_chosen_tab = curr_chosen_tab;
        }

        ImGui::EndTabItem();
      }
    }
    RepairsToTable(repairs);
    ImGui::EndTabBar();
  }
  if (ImGui::Button("Test Map")) {
    std::cout << "++++++++++++++++++++++++++++++++++++" << std::endl;

    for (auto& pair : repairs) {
      std::cout << "ID: " << pair.first << std::endl;
    }
  }

}

void RepairsView::RepairsToTable(const std::map<int, Repair>& retreived_repairs) {
    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable;
    static int selected = -1;
    if (ImGui::BeginTable("##states", 11, flags)) {
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Model", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Category", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Color", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Price", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Note", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Note Hidden", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Cust. Phone number", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Cust. Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthStretch);


        ImGui::TableHeadersRow();

        if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
          if (sort_specs->SpecsDirty)
          {
            printf("Sorted\n");
            repairs.clear();
            std::cout << sort_specs->Specs->SortDirection << std::endl;
            std::cout << sort_specs->Specs->ColumnIndex << std::endl;

            repairs = Database::RetreiveRepairsOfState(1, sort_specs->Specs->SortDirection, sort_specs->Specs->ColumnIndex);

            sort_specs->SpecsDirty = false;
          }
        for (auto& pair : retreived_repairs) {
            const bool is_selected = (selected == pair.first);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            char label[32];
            sprintf_s(label, "%d", pair.first);
            if (ImGui::Selectable(label, is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap)) {
                selected = pair.first;
            }
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::Button("Update repair")) {
                    *EditRepair::show_repair = true;
                    repair_to_init = pair.second;
                    edit_repair = std::make_shared<EditRepair>(repair_to_init, pair.first);
                }
                ImGui::EndPopup();
            }
            ImGui::TableNextColumn();
            ImGui::Text(pair.second.device.name.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(pair.second.category.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(pair.second.device.color.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%.2f", pair.second.price);
            ImGui::TableNextColumn();
            ImGui::Text(pair.second.visible_note.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(pair.second.hidden_note.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(pair.second.customer.phone.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(pair.second.customer.name.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(pair.second.state.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(pair.second.date.c_str());
            }
        }
        ImGui::EndTable();

}

std::shared_ptr<EditRepair> RepairsView::GetEditRepair() {
    return edit_repair;
}