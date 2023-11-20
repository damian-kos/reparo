// In the source file for EditRepair
#include "edit_repair.h"

int EditRepair::instance_count = 0;
bool* EditRepair::show_repair = new bool(false);

//EditRepair::EditRepair() { instance_count++;  std::cout << "EditRepair created " << instance_count << std::endl; }
EditRepair::EditRepair(Repair& repair, int passed_repair_id) : InsertRepair(repair), repair_id(passed_repair_id) {
    instance_count++;
    modal_message = "Confirm Update Repair";
    selected_state = repair.state;
    std::cout << "EditRepair created " << instance_count << std::endl;
}

EditRepair::~EditRepair() { instance_count--;  std::cout << "EditRepair destroyed " << instance_count << std::endl;  }

void EditRepair::StateSection() {
  static int selected = 1;
    static bool retreived = false;
    static std::map<int, std::string> states;
    imgui_decorator.SetTestValue(true);
    imgui_decorator.DecorateSeparatorText("STATE: ");
    if (!retreived) {
        states = db.GetRepairStates();
        retreived = !states.empty();
    }

    if (ImGui::BeginCombo("##States", states[selected].c_str()))
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
    // Debugging
    static std::string editing = "Currently editing: " + std::to_string(repair_id);
    ImGui::Text(editing.c_str());
    // End of debugging
    if (ImGui::Button("Update Repair")) {
        //repair = InitRepair();
        InitModal();
    }
}


void EditRepair::RunModal(Repair& repair){
    modals.SubmitConfirm(modal_message, repair, result);
    if (result == ConfirmResult::CONIFRM_SUBMIT) {
        if (CustomerModified()) {
            Customer temp_customer = InsertCustomer::InitCustomer();
            int customerID = db.QueryCustomerByPhone(temp_customer.phone);
            if (customerID > 0) {
                db.UpdateCustomer(temp_customer, customerID); 
                }
            if (customerID == 0) {
                db.UpdateCustomer(temp_customer, db.GetIDForID(repair_id, "repairs"));
            }
            if (customerID != db.GetIDForID(repair_id, "repairs") && customerID >0) {
                // Change ID of customer_id in repairs table to ID of other existing customer in database
            }
        }
        if (RepairModified()) {
            Repair temp_repair = InitRepair();
            db.UpdateRepair(temp_repair, repair_id);
        }
        //db.InsertRepair(repair);
        //ResetFields();
        result = ConfirmResult::CONIFRM_IDLE;

    }
}

Repair EditRepair::InitRepair() {
    Device device(model.input.buffer, color.input.buffer);
    Repair init_repair(InitCustomer(), device, category.input.buffer, price, visible_note.buffer, hidden_note.buffer, selected_state);
    return init_repair;
}

void EditRepair::TestButton() {
    if (ImGui::Button("Test button")) {
        std::string modified = RepairModified() ? "true" : "false";
        std::string modified_c = CustomerModified() ? "Cust true" : "Cust false";

        
        std::cout << modified << std::endl;
        std::cout << modified_c << std::endl;

    }
}

bool EditRepair::CustomerModified() {
    std::cout << "Phone " << (repair.customer.phone == std::string(phone.input.buffer)) << std::endl;
    std::cout << "Name " << (repair.customer.name == std::string(name.buffer)) << std::endl;
    std::cout << "Surname " << (repair.customer.surname == std::string(surname.buffer)) << std::endl;
    std::cout << "Email " << (repair.customer.email == std::string(email.buffer)) << std::endl;

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