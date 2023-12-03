#include "repairs.h"

RepairsView::RepairsView() { 
    std::cout << "RepairsView Created empty" << std::endl;

    names = Database::GetRepairStatesNames();
    opened = new bool[5];
    for (int i = 0; i < 5; i++) {
      opened[i] = true;
    }
    repairs = Database::RetreiveRepairsOfState(curr_chosen_tab+1);
}

RepairsView::~RepairsView() { std::cout << "RepairsView dESTROYED " << std::endl; }

void RepairsView::Render() {
  //if (ImGui::Button("Test button")) {
  //  for (auto& name : names)
  //    std::cout << name << std::endl;
  //}
  static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_AutoSelectNewTabs;
  if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
  {
    for (int n = 0; n < names.size(); n++) {
      if (opened[n] && ImGui::BeginTabItem(names[n].c_str(), &opened[n], ImGuiTabItemFlags_None))
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
}

void RepairsView::RepairsToTable(RepairsSort& retreived_repairs) {
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
            repairs.repairs.clear();
            repairs.repairs_order.clear();

            repairs = Database::RetreiveRepairsOfState(curr_chosen_tab+1, 
                                                       sort_specs->Specs->SortDirection, 
                                                       sort_specs->Specs->ColumnIndex);

            sort_specs->SpecsDirty = false;
          }
        for (auto& pair : retreived_repairs.repairs_order) {
          //const bool is_selected = (selected == pair.first);
          const bool is_selected = (selected == pair);
          retreived_repairs.repairs[1];
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          char label[32];
          sprintf_s(label, "%d", pair);
          if (ImGui::Selectable(label, is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap)) {
              selected = pair;
          }
          if (ImGui::BeginPopupContextItem()) {
              if (ImGui::Button("Update repair")) {
                  *EditRepair::show_repair = true;
                  repair_to_init = retreived_repairs.repairs[pair];
                  edit_repair = std::make_shared<EditRepair>(repair_to_init, pair);
              }
              ImGui::EndPopup();
          }
          ImGui::TableNextColumn();
          ImGui::Text(retreived_repairs.repairs[pair].device.name.c_str());
          ImGui::TableNextColumn();
          ImGui::Text(retreived_repairs.repairs[pair].category.c_str());
          ImGui::TableNextColumn();
          ImGui::Text(retreived_repairs.repairs[pair].device.color.c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%.2f", retreived_repairs.repairs[pair].price);
          ImGui::TableNextColumn();
          ImGui::Text(retreived_repairs.repairs[pair].visible_note.c_str());
          ImGui::TableNextColumn();
          ImGui::Text(retreived_repairs.repairs[pair].hidden_note.c_str());
          ImGui::TableNextColumn();
          ImGui::Text(retreived_repairs.repairs[pair].customer.phone.c_str());
          ImGui::TableNextColumn();
          ImGui::Text(retreived_repairs.repairs[pair].customer.name.c_str());
          ImGui::TableNextColumn();
          ImGui::Text(retreived_repairs.repairs[pair].state.c_str());
          ImGui::TableNextColumn();
          ImGui::Text(retreived_repairs.repairs[pair].date.c_str());
          }
        }
        ImGui::EndTable();

}

std::shared_ptr<EditRepair> RepairsView::GetEditRepair() {
    return edit_repair;
}