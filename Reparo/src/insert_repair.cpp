#include "insert_repair.h"

InsertRepair::InsertRepair() : InsertCustomer() { 
modal_message = "Confirm Repair Details"; 
std::cout << "InsertRepair created " << std::endl; 
}

InsertRepair::InsertRepair(Repair& repair_) : InsertCustomer(repair_.customer), 
                                              repair(repair_) {
InsertCustomer::CopyToBuffer(model.input.buffer,
                             repair_.device.name.c_str(),
                             model.input.valid,
                             [&]() { return ChkBufferInDb("##Model", model.input.buffer); });
InsertCustomer::CopyToBuffer(category.input.buffer,
                             repair_.category.c_str(),
                             category.input.valid,
                             [&]() { return ChkBufferInDb("##Category", category.input.buffer); });
InsertCustomer::CopyToBuffer(color.input.buffer,
                             repair_.device.color.c_str(),
                             color.input.valid,
                             [&]() { return ChkBufferInDb("##Color", color.input.buffer); });
strcpy(visible_note.buffer, repair_.visible_note.c_str());
strcpy(hidden_note.buffer, repair_.hidden_note.c_str());
price = repair_.price;
strcpy(sn_imei.buffer, repair_.sn_imei.c_str());

}

void InsertRepair::Render() {
CustomerSection();
DeviceSection();
SerialNumSection();
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
  ImGui::InputTextWithPopup("##PartialPhone", "Phone number...", phone, 
                            [&]() { return LenValidation(phone.input.buffer, 8); }, 
                            &selected, nullptr, nullptr);    
}

void InsertRepair::DeviceSection() {
  ImGui::SeparatorDecorator("DEVICE: ", DeviceFieldsValidated());
  ImGui::InputTextWithPopup("##Model", "Model of device...", model, 
                            [&]() { return ChkBufferInDb("##Model", model.input.buffer); }, NULL, NULL, &feedback);
  ImGui::InputTextWithPopup("##Category", "Category...", category, 
                            [&]() { return ChkBufferInDb("##Category", 
                            category.input.buffer); });
  ImGui::InputTextWithPopup("##Color", "Color...", color, 
                            [&]() { return ChkBufferInDb("##Color", color.input.buffer); }, 
                            nullptr, &model);
  CustomDeviceFeedback();
  
  //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Custom DEVICE Start

  //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Custom DEVICE End

  //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Debugging
  //
  //ImGui::SameLine(); ImGui::Text(category.input.valid ? "true" : "false");
  //ImGui::SameLine(); ImGui::Text(color.input.valid ? "true" : "false");
  // 
  //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
}

void InsertRepair::CustomDeviceFeedback() {
  static std::string msg;
  if (feedback) {
    if (!model.input.valid && strlen(model.input.buffer) >= 3) {
      msg = ("New custom device will be added.");
      custom_device = true;
    }
    else {
      msg = "";
      custom_device = false;
    }
  }
  ImGui::Text(msg.c_str());
  if (msg != "") {
    ImGui::SameLine();
    ImGui::HelpMarker("If a model of a device which does not exist in database, you can add a custom device to database. These devices will not appear on usual popup of Model field.");
  }
}

void InsertRepair::NotesSection() {
  ImGui::SeparatorDecorator("NOTES: ", true);
  ImGui::InputTextWithHintExt("##VisibleN", "Note...", visible_note, nullptr, nullptr);
  ImGui::InputTextWithHintExt("##HiddenNote", "Note for shop...", hidden_note, nullptr, nullptr);
}

void InsertRepair::PriceSection() {
  ImGui::SeparatorDecorator("PRICE: ", (price > 0) ? true : false);
  ImGui::InputDouble("##input float", &price, 0.1f, 1.0f, "%.2f");
}

void InsertRepair::SerialNumSection() {
  ImGui::SeparatorDecorator("SERIAL: ", true);
  ImGui::InputTextWithHintExt("##SN", "IMEI / SN...", sn_imei, nullptr, nullptr);
}

void InsertRepair::StateSection() {

}

void InsertRepair::ResetOnEmptyMain() {
  InsertCustomer::ResetOnEmptyMain();
  if (strlen(model.input.buffer) == 0 && !model_field_empty) {
    category = HintInputFieldsW_Popup();
    color = HintInputFieldsW_Popup();
  }
  if (strlen(model.input.buffer) > 0) { model_field_empty = false; }
}

bool InsertRepair::DeviceFieldsValidated() {
  if (custom_device && category.input.valid) {
    return custom_device;
  }
  return (model.input.valid && category.input.valid && color.input.valid);
}

bool InsertRepair::RepairValidated() {
  return (DeviceFieldsValidated() && FieldsValidated() && price > 0);
}

bool InsertRepair::ChkBufferInDb(const char* label, const char* buffer) {
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
  if (ImGui::Button("Test Repair")) {
    repair = InitRepair();
    std::cout << "Repair: " << repair.customer.phone << " | " << repair.device.name << std::endl;
    Notify();
  }
}

void InsertRepair::InsertRepairButton() {
  if (ImGui::Button("Insert Repair")) {
      repair = InitRepair();
      InitModal();
  }
}

Repair InsertRepair::InitRepair() {
  Device device(model.input.buffer, color.input.buffer);
  Repair init_repair(InitCustomer(), device, category.input.buffer, price, visible_note.buffer, hidden_note.buffer, sn_imei.buffer);
  return init_repair;
}

void InsertRepair::InitModal() {
  ModalController::RenderModal(modal_message);
}

// Opens modal window and waits for users' click. Inserts repair to DB.
void InsertRepair::RunModal(Repair& repair) {
  ModalController::ModalConfirm(modal_message, repair, result);
  if (result == ConfirmResult::CONIFRM_SUBMIT) {
      int customerID = Database::QueryCustomerIDByPhone(repair.customer.phone);
      if (customerID == 0) {
          Database::InsertCustomer(repair.customer, nullptr); // Insert Customer if doesnt exist
      }
      Database::InsertRepair(repair);
      Notify();
      ResetFields();
      result = ConfirmResult::CONIFRM_IDLE;
  }
}

void InsertRepair::InsertCustButton() {

}

void InsertRepair::ResetFields() {
  InsertCustomer::ResetFields();
  model = HintInputFieldsW_Popup();
  category = HintInputFieldsW_Popup();
  color = HintInputFieldsW_Popup();
  visible_note = HintInputField();
  hidden_note = HintInputField();
  price = 0;
  //repair = Repair();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Debugging
void InsertRepair::TestButton() {

  //if (ImGui::Button("Print")) {
  //}
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Debugging

void InsertRepair::Attach(IObserver* observer) {
  list_observer.push_back(observer);
}

void InsertRepair::Detach(IObserver* observer) {
  list_observer.erase(std::remove(list_observer.begin(), list_observer.end(), observer), list_observer.end());
}

void InsertRepair::Notify() {
  for (IObserver* observer : list_observer) {
    std::cout << "Repair PTR: " << &repair << std::endl;
    observer->Update(1, &repair);
  }
}

