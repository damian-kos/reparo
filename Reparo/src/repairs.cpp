#include "repairs.h"

RepairsView::RepairsView() { 
    std::cout << "RepairsView Created empty" << std::endl;

    names = Database::GetRepairStatesNames();
    opened = new bool[names.size()];
    //for (int i = 0; i < names.size(); i++) {
    //  opened[i] = true;
    //}
    all_repairs_of_state = Database::RetreiveRepairsOfState(curr_chosen_tab+1);
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
          all_repairs_of_state = Database::RetreiveRepairsOfState(n + 1);
          prev_chosen_tab = curr_chosen_tab;
        }

        ImGui::EndTabItem();
      }
    }
    RepairsToTable(all_repairs_of_state, curr_chosen_tab, true);
    ImGui::EndTabBar();
  }
  RunModal();
}

void RepairsView::RepairsToTable(RepairsSort& retreived_repairs, int& state, bool sorting) {
  static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable;
  static int selected = -1;
  if (ImGui::BeginTable("##states", 11, flags)) {
    std::vector<std::string> col_names = { "ID", "Model", "Category", "Color", 
                                           "Price", "Note", "Note hidden", 
                                           "Cust. Phone Number", "Cust. Name", 
                                           "State", "Date" };
    
    for(auto& name : col_names)
      ImGui::TableSetupColumn(name.c_str(), ImGuiTableColumnFlags_WidthFixed);

    ImGui::TableHeadersRow();
    if (sorting)
      if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
        if (sort_specs->SpecsDirty)
        {
          printf("Sorted\n");
          retreived_repairs.repairs.clear();
          retreived_repairs.repairs_order.clear();
          retreived_repairs = Database::RetreiveRepairsOfState(state + 1,
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
        if (ImGui::Button("Delete")) {
          modal = true;
          repair_to_init = retreived_repairs.repairs[pair];
          modal_on_id = pair;
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

void RepairsView::RunModal() {
  if (modal) {
    ModalController::RenderModal("Delete this repair?");
    modal = false;
  }
  ModalController::SubmitConfirm("Delete this repair?", repair_to_init, deletion);
  if (deletion == ConfirmResult::CONIFRM_SUBMIT) {
    Database::DeleteRepair(modal_on_id);
    all_repairs_of_state = Database::RetreiveRepairsOfState(curr_chosen_tab + 1);
    deletion = ConfirmResult::CONIFRM_IDLE;
  }
}


std::shared_ptr<EditRepair> RepairsView::GetEditRepair() {
    return edit_repair;
}