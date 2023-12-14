// In the source file for EditRepair
#include "edit_repair.h"

int EditRepair::instance_count = 0;

EditRepair* EditRepair::instance = nullptr;
bool* EditRepair::show_repair = new bool(false);

EditRepair::EditRepair(Repair& repair, int passed_repair_id) : InsertRepair(repair), repair_id(passed_repair_id) {
    instance_count++;
    modal_message = "Confirm Update Repair";
    selected_state = repair.state;
    updates = Database::RetreiveRepairUdpdates(passed_repair_id);
    std::cout << "EditRepair created " << instance_count << std::endl;
    if (instance != nullptr) {
      list_observer.clear();
      observers_attached = false;
      delete instance;
    }
    instance = this;
}

EditRepair::~EditRepair() { instance_count--;  std::cout << "EditRepair destroyed " << instance_count << std::endl;  }

void EditRepair::Render() {
  if (ImGui::BeginTable("##Table2", 2, ImGuiTableFlags_BordersInnerV)) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    InsertRepair::Render();
    ImGui::TableNextColumn();
    ImGui::SeparatorText("UPDATES: ");
    if (ImGui::BeginTable("##Table3", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
      for (auto& update : updates) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::SeparatorTextAlignR(update.date.c_str());
        //ImGui::TableNextRow();
        //ImGui::TableNextColumn();
        ImGui::Text(update.note.c_str());
        ImGui::Spacing();
        ImGui::Spacing();
      }
    }
    ImGui::EndTable();
  }
  ImGui::EndTable();
}

void EditRepair::StateSection() {
  static int selected = 1;
  static bool retreived = false;
  static std::unordered_map<int, std::string> states;

  ImGui::SeparatorDecorator("STATE: ", true);
  if (!retreived) {
    states = Database::GetRepairStates();
    retreived = !states.empty();
  }

  if (ImGui::BeginCombo("##States", selected_state.c_str()))
  {
    for (auto& pair : states) {
      const bool is_selected = (selected == pair.first);
      if (ImGui::Selectable(states[pair.first].c_str(), is_selected)) {
        selected = pair.first;
        selected_state = pair.second;
      }
      if (is_selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
}

void EditRepair::InsertRepairButton() {
  //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Debugging
  //static std::string editing = "Currently editing: " + std::to_string(repair_id);
  //ImGui::Text(editing.c_str());
  //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    if (ImGui::Button("Update Repair")) {
      if (CustomerModified()) 
        t_customer = std::make_shared<Customer>(InitCustomer());
      if (RepairModified())
        t_repair = std::make_shared<Repair>(InitRepair());
      InitModal();
    }
}

void EditRepair::RunModal(Repair& repair){
  ModalController::SubmitConfirm(modal_message, repair, t_repair, t_customer, result);
  if (result == ConfirmResult::CONIFRM_SUBMIT) {
    if (CustomerModified()) {
      Customer temp_customer = InsertCustomer::InitCustomer();
      int customerID = Database::QueryCustomerIDByPhone(temp_customer.phone);
      if (customerID > 0) {
          Database::UpdateCustomer(temp_customer, customerID); 
          }
      if (customerID == 0) {
          Database::UpdateCustomer(temp_customer, Database::GetIDForID(repair_id, "repairs"));
      }
      if (customerID != Database::GetIDForID(repair_id, "repairs") && customerID >0) {
          // Change ID of customer_id in repairs table to ID of other existing customer in database
      }
    }
    if (RepairModified()) {
      Repair temp_repair = InitRepair();
      Database::UpdateRepair(temp_repair, repair_id); // this can be changed to pointer, since we already have t_repair & t_customer
    }
    if(RepairModified() || CustomerModified())
      UpdateSummary();
    //ResetFields();
    result = ConfirmResult::CONIFRM_IDLE;
  }
}

Repair EditRepair::InitRepair() {
  Device device(model.input.buffer, color.input.buffer);
  Repair init_repair(InitCustomer(), device, category.input.buffer, price, visible_note.buffer, hidden_note.buffer, selected_state, str_date);
  return init_repair;
}

bool EditRepair::CustomerModified() {
    return !(repair.customer.phone == std::string(phone.input.buffer) &&
        repair.customer.name == std::string(name.buffer) &&
        repair.customer.surname == std::string(surname.buffer) &&
        repair.customer.email == std::string(email.buffer));
}

bool EditRepair::RepairModified() {
    double tolerance = 1e-3;
    return !(
        repair.device.name == std::string(model.input.buffer) &&
        repair.category == std::string(category.input.buffer) &&
        repair.device.color == std::string(color.input.buffer) &&
        repair.visible_note == std::string(visible_note.buffer) &&
        repair.hidden_note == std::string(hidden_note.buffer) &&
        (std::abs(repair.price - price) < tolerance) &&
        repair.state == selected_state
        );
}

bool EditRepair::RepairValidated()
{
  return ((CustomerModified() || RepairModified()) && InsertRepair::RepairValidated());
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Debugging
void EditRepair::TestButton() {
  if (ImGui::Button("Test button")) {
    std::cout << "Repair ID:" << repair_id << std::endl;
  }
    //std::string modified = RepairModified() ? "true" : "false";
    //std::string modified_c = CustomerModified() ? "true" : "false";
    //std::string validated_r = InsertRepair::RepairValidated() ? "true" : "false";

    //ImGui::Text("Repair modified: %s", modified.c_str());
    //ImGui::Text("Customer modified: %s", modified_c.c_str());
    //ImGui::Text("Repair validated: %s", validated_r.c_str());

}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 

// *****************************************************************************
void EditRepair::UpdateSummary() {
  std::string summary = "";
  if (CustomerModified()) {
    summary += customer.phone != t_customer->phone ? "Phone updated: \n" + customer.phone + "  >>  " + t_customer->phone + "\n" : "";
    summary += customer.name != t_customer->name ? "Name updated: \n" + customer.name + "  >>  " + t_customer->name + "\n" : "";
    summary += customer.surname != t_customer->surname ? "Surname updated: \n" + customer.surname + "  >>  " + t_customer->surname + "\n" : "";
    summary += customer.email != t_customer->email ? "Email updated: \n" + customer.email + "  >>  " + t_customer->email + "\n" : "";
  }
  if (RepairModified()) {
    summary += repair.device.name != t_repair->device.name ? "Device updated: " + repair.device.name + "  >>  " + t_repair->device.name + "\n" : "";
    summary += repair.category != t_repair->category ? "Category updated: " + repair.category + "  >>  " + t_repair->category + "\n" : "";
    summary += repair.device.color != t_repair->device.color ? "Color updated: " + repair.device.color + "  >>  " + t_repair->device.color + "\n" : "";
    summary += repair.state != t_repair->state ? "State updated: " + repair.state + "  >>  " + t_repair->state + "\n" : "";
    double tolerance = 1e-3;
    if (!(std::abs(repair.price - t_repair->price) < tolerance)) {
      std::stringstream ss;
      ss << "Price updated: " << std::fixed << std::setprecision(2) << repair.price << " >> "
        << std::fixed << std::setprecision(2) << t_repair->price;

      summary += ss.str();
    }
  }
  Database::InsertRepairUpdateDesc(repair_id, summary);
  updates = Database::RetreiveRepairUdpdates(repair_id);
  
}

EditRepair* EditRepair::Get() {
  return instance;
}