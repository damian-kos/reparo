#include "repairs.h"


RepairsView::RepairsView() { 
    std::cout << "RepairsView Created empty" << std::endl;
    names = Database::GetSimpleTableNames();
    opened = new bool[names.size()];
}

RepairsView::~RepairsView() { std::cout << "RepairsView Destroyed " << std::endl; }

void RepairsView::Render() {
  DateDirection(rv_relation); ImGui::SameLine();  
  CalendarsRender(rv_relation.number, rv_date_from, rv_date_to);
  OnDatesChanged(rv_date_from, rv_relation.number, rv_date_to,
                 rv_all_repairs_by_query, curr_chosen_tab+1);
  OnDatesDirectionChange(rv_date_from, rv_relation, rv_date_to,
                         rv_all_repairs_by_query);
  StatesTabBar();
  RunModal();
}

void RepairsView::StatesTabBar()
{
  static int cur_state;
  static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_AutoSelectNewTabs;
  if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
  {
    for (int n = 0; n < names.size(); n++) {
      if (opened[n] && ImGui::BeginTabItem(names[n].c_str(), &opened[n], ImGuiTabItemFlags_None)) {
        curr_chosen_tab = n;
        if (curr_chosen_tab != prev_chosen_tab) {
          std::cout << "Reload repairs for this tab: " << n + 1 << std::endl;
          printf("Called from RepairsView StatesTabBar\n");
          rv_all_repairs_by_query = Database::RetreiveRepairsByDate(rv_date_from.str, rv_relation.number, rv_date_to.str, n + 1);
          prev_chosen_tab = curr_chosen_tab;
        }
        ImGui::EndTabItem();
      }
    }
    RepairsToTable(rv_date_from, rv_relation.number, rv_date_to, 
                   curr_chosen_tab+1, rv_all_repairs_by_query, rv_table_select);
    ImGui::EndTabBar();
  }
}

void RepairsView::OnDatesChanged(r_tm & date_from, int& relation, r_tm & date_to,  
                                 RepairsSort& repairs, int state) {

  if (!(relation == 3)) {
    if (date_to.str) {
      printf("DATE TO POINTER DELETED \n");
      delete date_to.str;
      date_to.str = nullptr;
    }
  }
  if (date_from.changed) {
    printf("STATE ID IS: %d\n", state);
    if (date_to.str) 
      repairs = Database::RetreiveRepairsByDate(date_from.str, relation, 
                                                date_to.str, state);
    else
      repairs = Database::RetreiveRepairsByDate(date_from.str, relation, 
                                                nullptr, state);
    RefreshRepairs();
    date_from.changed = false;
  }
  if (date_to.changed) {
    if (date_from.str) {
      repairs = Database::RetreiveRepairsByDate(date_from.str, 3, date_to.str, state);
      RefreshRepairs();
    }
    date_to.changed = false;
  }
}

void RepairsView::OnDatesDirectionChange(r_tm& date_from, date_rel& relation, r_tm& date_to, RepairsSort& repairs) {
  if (relation.changed) {
    printf("RELATION CHANGED\n");
    if (relation.number == 3 && date_to.str == nullptr) {
      //ImGui::SetDateToday(&date_to._tm);
      DateToStr(date_to);
      DateToStr(date_from);

      date_to.changed = true;
      printf("RELATION DATE SET %s\n", date_to.str->c_str());
    }
    else {
      DateToStr(date_from);
      date_from.changed = true;
    }
  }
  relation.changed = false;

}

void RepairsView::Calendar(int ID, r_tm& date) {
  ImGui::PushID(ID);
  ImGui::PushItemWidth(250);
  if (ImGui::DateChooser("##From", date._tm)) {
    DateToStr(date);
    date.changed = true;
  }
  ImGui::PopItemWidth();
  ImGui::PopID();
}

void RepairsView::LeadingZero(int& date_int, std::string& date_str) {
  if (date_int >= 10) {
    date_str = std::to_string(date_int);
  }
  else {
    date_str = "0" + std::to_string(date_int);
  }
}

void RepairsView::DateToStr(r_tm& date) {
  date.str = new std::string;
  std::string year = std::to_string(date._tm.tm_year + 1900);
  std::string month;
  int month_int = date._tm.tm_mon + 1;
  LeadingZero(month_int, month);
  std::string day;
  LeadingZero(date._tm.tm_mday, day);

  *date.str = year + "-" + month + "-" + day;
}

void RepairsView::DateDirection(date_rel& rel) {
  static std::unordered_map<int, std::string> date_relation = {
    {1, "Before"},
    {2, "After"},
    {3, "Between"}
  };
  ImGui::PushItemWidth(100);
  if (ImGui::BeginCombo("##Relation", date_relation[rel.number].c_str())) {
    for (auto& pair : date_relation) {
      const bool is_selected = (rel.number == pair.first);
      if (ImGui::Selectable(date_relation[pair.first].c_str(), is_selected)) {
        if (rel.number != pair.first) {
          rel.number = pair.first;
          rel.changed = true;
        }
      }
    }
    ImGui::EndCombo();
  }

  ImGui::PopItemWidth();
}

void RepairsView::CalendarsRender(const int& relation, r_tm& date_from, r_tm& date_to) {
  Calendar(1, date_from); ImGui::SameLine();
  ImGui::BeginDisabled(!(relation == 3));
  Calendar(2, date_to);
  ImGui::EndDisabled();
}


void RepairsView::RepairsToTable(r_tm& date_from, int& relation, r_tm& date_to, const int state, RepairsSort& retreived, int& selected) {

  static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable;
  if (ImGui::BeginTable("##repairs", 12, flags)) {
    std::vector<std::string> col_names = { "ID", "Model", "Category", "Color", 
                                           "Price", "Note", "Note hidden", 
                                           "Cust. Phone Number", "Cust. Name", 
                                           "State", "Date", "SN_IMEI"};
    
    for(auto& name : col_names)
      ImGui::TableSetupColumn(name.c_str(), ImGuiTableColumnFlags_WidthFixed);

    ImGui::TableHeadersRow();

    if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs()) {
      if (sort_specs->SpecsDirty) {
        printf("Sorted\n");
        retreived.repairs.clear();
        retreived.repairs_order.clear();
        retreived = Database::RetreiveRepairsByDate(date_from.str,
          relation, date_to.str,
          state,
          sort_specs->Specs->SortDirection,
          sort_specs->Specs->ColumnIndex);

        sort_specs->SpecsDirty = false;
      }
    }
    for (auto& pair : retreived.repairs_order) {
      const bool is_selected = (selected == pair);
      retreived.repairs[1];
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      char label[32];
      sprintf_s(label, "%d", pair);
      if (ImGui::Selectable(label, is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap)) {
          selected = pair;
      }
      if (ImGui::BeginPopupContextItem()) {
        if (ImGui::Button("Update repair")) {
          EditRepair::Set(std::make_shared<EditRepair>(retreived.repairs[pair], pair));
          *EditRepair::show_repair = true;
        }
        if (ImGui::Button("Delete")) {
          modal = true;
          repair_to_init = retreived.repairs[pair];
          modal_on_id = pair;
        }
        ImGui::EndPopup();
      }
      ImGui::TableNextColumn();
      ImGui::Text(retreived.repairs[pair].device.name.c_str());
      ImGui::TableNextColumn();
      ImGui::Text(retreived.repairs[pair].category.c_str());
      ImGui::TableNextColumn();
      ImGui::Text(retreived.repairs[pair].device.color.c_str());
      ImGui::TableNextColumn();
      ImGui::Text("%.2f", retreived.repairs[pair].price);
      ImGui::TableNextColumn();
      ImGui::Text(retreived.repairs[pair].visible_note.c_str());
      ImGui::TableNextColumn();
      ImGui::Text(retreived.repairs[pair].hidden_note.c_str());
      ImGui::TableNextColumn();
      ImGui::Text(retreived.repairs[pair].customer.phone.c_str());
      ImGui::TableNextColumn();
      ImGui::Text(retreived.repairs[pair].customer.name.c_str());
      ImGui::TableNextColumn();
      ImGui::Text(retreived.repairs[pair].state.c_str());
      ImGui::TableNextColumn();
      ImGui::Text(retreived.repairs[pair].date.c_str());
      ImGui::TableNextColumn();
      ImGui::Text(retreived.repairs[pair].sn_imei.c_str());
      }
    ImGui::EndTable();
  }
}

void RepairsView::RunModal() {
  if (modal) {
    ModalController::RenderModal("Delete this repair?");
    modal = false;
  }
  ModalController::ModalConfirm("Delete this repair?", repair_to_init, deletion);
  if (deletion == ConfirmResult::CONIFRM_SUBMIT) {
    //Database::DeleteRepair(modal_on_id);
    //RefreshRepairs();
    Notify();

    deletion = ConfirmResult::CONIFRM_IDLE;
  }
}


std::shared_ptr<EditRepair> RepairsView::GetEditRepair() {
    return edit_repair;
}

void RepairsView::RefreshRepairs() {
  rv_all_repairs_by_query = Database::RetreiveRepairsByDate(rv_date_from.str, rv_relation.number, rv_date_to.str, curr_chosen_tab+1);
}

void RepairsView::Attach(IObserver* observer) {
  list_observer.push_back(observer);
}

void RepairsView::Detach(IObserver* observer) {
  list_observer.erase(std::remove(list_observer.begin(), list_observer.end(), observer), list_observer.end());
}

void RepairsView::Notify() {
  for (IObserver* observer : list_observer) {
    observer->Update(1, nullptr);
  }
}
void RepairsView::Update(const int& passed_int, Repair* repair) {
  RefreshRepairs();
}
