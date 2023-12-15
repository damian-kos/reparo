#include "insert_repair.h"

InsertRepair::InsertRepair() : InsertCustomer() { 
modal_message = "Confirm Repair Details"; 
std::cout << "InsertRepair created " << std::endl; 
}

InsertRepair::InsertRepair(Repair& repair_) : InsertCustomer(repair_.customer), 
                                              repair(repair_) {
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
static tm date_from = {};
ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
ImGui::DateChooser("##From", date_from);
ImGui::PopItemWidth();
CustomerSection();
DeviceSection();
NotesSection();
PriceSection();
StateSection();
InsertRepairButtonEnabler();
TestButton();
}

void InsertRepair::ResetOnEmptyMain(){
  InsertCustomer::ResetOnEmptyMain();
  if (strlen(model.input.buffer) == 0 && !model_field_empty) {
    //model = HintInputFieldsW_Popup();
    category = HintInputFieldsW_Popup();
    color = HintInputFieldsW_Popup();
  }
  if (strlen(model.input.buffer) > 0) { model_field_empty = false; }
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
    
  //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Debugging
  //                      
  //ImGui::Text(model.input.validated ? "true" : "false");
  //ImGui::SameLine(); ImGui::Text(category.input.validated ? "true" : "false");
  //ImGui::SameLine(); ImGui::Text(color.input.validated ? "true" : "false");
  // 
  //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
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
      Notify();
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

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Debugging
void InsertRepair::TestButton() {
//  if (ImGui::Button("Test button")) {
//  }
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
    observer->Update(1);
  }
}

