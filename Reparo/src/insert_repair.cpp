#include "insert_repair.h"

InsertRepair::InsertRepair() : InsertCustomer() { modal_message = "Confirm Repair Details"; std::cout << "InsertRepair created " << std::endl; }

InsertRepair::InsertRepair(Repair& repair_) : InsertCustomer(repair_.customer), repair(repair_) {
    InsertCustomer::CopyToBuffer(model.input.buffer,
        repair_.device.name.c_str(),
        model.input.validated,
        [&]() { return BufferQueryOnDatabase("##Model", model.input.buffer); });
    InsertCustomer::CopyToBuffer(category.input.buffer,
        repair_.category.c_str(),
        category.input.validated,
        [&]() { return BufferQueryOnDatabase("##Category", category.input.buffer); });
    InsertCustomer::CopyToBuffer(color.input.buffer,
        repair_.device.color.c_str(),
        color.input.validated,
        [&]() { return BufferQueryOnDatabase("##Color", color.input.buffer); });
    strcpy(visible_note.buffer, repair_.visible_note.c_str());
    strcpy(hidden_note.buffer, repair_.hidden_note.c_str());
    price = repair_.price;
}

void InsertRepair::Render() {
    if (ImGui::DateChooser("Date Chooser##MyDateChooser", date, "%d/%m/%Y")) {
         //A new date has been chosen
        fprintf(stderr,"A new date has been chosen exacty now: \"%.2d-%.2d-%.4d\"\n",date.tm_mday,date.tm_mon+1,date.tm_year+1900);
    }
    ImGui::Text("Chosen date: \"%.2d-%.2d-%.4d\"", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900);

    ////ImGui::DateChooser("##Date", date);
    //ImGui::Text("Chosen date: \"%.4d-%.2d-%.2d %.2d:%.2d:%.2d\"", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
    CustomerSection();
    DeviceSection();
    NotesSection();
    PriceSection();
    StateSection();
    InsertRepairButtonEnabler();
    TestButton();
}

void InsertRepair::CustomerSection() {
    InsertCustomer::Render();
}

void InsertRepair::PhoneFieldSection() {
    CustomerSelectedOnPopup();
    ImGui::InputTextWithPopup("##PartialPhone", "Phone number...", phone, [&]() { return LenValidation(phone.input.buffer, 8); }, &selected, nullptr, nullptr);    
}
void InsertRepair::DeviceSection() {
    ImGui::SeparatorDecorator("DEVICE: ", DeviceFieldsValidated());
    ImGui::InputTextWithPopup("##Model", "Model of device...", model, [&]() { return BufferQueryOnDatabase("##Model", model.input.buffer); }, nullptr, nullptr, nullptr);
    ImGui::InputTextWithPopup("##Category", "Category...", category, [&]() { return BufferQueryOnDatabase("##Category", category.input.buffer); }, nullptr, nullptr, nullptr);
    ImGui::InputTextWithPopup("##Color", "Color...", color, [&]() { return BufferQueryOnDatabase("##Color", color.input.buffer); }, nullptr, &model, nullptr);
    
    //Debugging
    ImGui::Text(model.input.validated ? "true" : "false");
    ImGui::SameLine(); ImGui::Text(category.input.validated ? "true" : "false");
    ImGui::SameLine(); ImGui::Text(color.input.validated ? "true" : "false");
    //
}

void InsertRepair::NotesSection() {
    ImGui::SeparatorDecorator("NOTES: ", true);
    ImGui::InputTextWithHintExt("##VisibleN", "Note..", visible_note, nullptr, nullptr);
    ImGui::InputTextWithHintExt("##HiddenNote", "Note for shop...", hidden_note, nullptr, nullptr);
}

void InsertRepair::PriceSection() {
    ImGui::SeparatorDecorator("PRICE: ", (price > 0) ? true : false);
    ImGui::InputDouble("##input float", &price, 0.1f, 1.0f, "%.2f");
}

void InsertRepair::StateSection() {

}


bool InsertRepair::DeviceFieldsValidated() {
    return (model.input.validated && category.input.validated && color.input.validated);
}

bool InsertRepair::RepairValidated() {
    return (DeviceFieldsValidated() && FieldsValidated() && price > 0);
}

bool InsertRepair::BufferQueryOnDatabase(const char* label, const char* buffer) {
    return Database::GetBoolForValue(label, buffer);
}

void InsertRepair::InsertRepairButtonEnabler() {
    ImGui::SeparatorDecorator("SUBMIT: ", true);

    if (!RepairValidated()) {
        ImGui::BeginDisabled(true);
    }
    InsertRepairButton();
    if (!RepairValidated()) {
        ImGui::EndDisabled();
    }
    RunModal(repair); // This is where insertion of Repair to db is called
}

void InsertRepair::InsertRepairButton() {
    if (ImGui::Button("Insert Repair")) {
        repair = InitRepair();
        InitModal();
    }
}

Repair InsertRepair::InitRepair() {
    Device device(model.input.buffer, color.input.buffer);
    Repair init_repair(InitCustomer(), device, category.input.buffer, price, visible_note.buffer, hidden_note.buffer, str_date);
    return init_repair;
}

void InsertRepair::InitModal() {
    ModalController::RenderModal(modal_message);
}

void InsertRepair::RunModal(Repair& repair) {
    ModalController::SubmitConfirm(modal_message, repair, result);
    if (result == ConfirmResult::CONIFRM_SUBMIT) {
       int customerID = Database::QueryCustomerIDByPhone(repair.customer.phone);
       if (customerID == 0) {
            Database::InsertCustomer(repair.customer, nullptr); // Insert Customer if doesnt exist
       }
       Database::InsertRepair(repair);
       //ResetFields();
       result = ConfirmResult::CONIFRM_IDLE;

    }
}

void InsertRepair::SubmitButton() {

}

void InsertRepair::ResetFields() {
    InsertCustomer::ResetFields();
    model = HintInputFieldsW_Popup();
    category = HintInputFieldsW_Popup();
    color = HintInputFieldsW_Popup();
    visible_note = HintInputField();
    hidden_note = HintInputField();
    price = 0;
}

void InsertRepair::TestButton() {
    if (ImGui::Button("Test button")) {
        ImGui::SetDateToday(&date);
        char time[64];
        sprintf(time, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
            date.tm_year + 1900, date.tm_mon + 1, date.tm_mday,
            date.tm_hour, date.tm_min, date.tm_sec);

        std::string str_date(time);
        std::cout << str_date << std::endl;
    }
}