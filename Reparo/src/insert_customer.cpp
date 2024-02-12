#include "insert_customer.h"

InsertCustomer::InsertCustomer() { 
  std::cout << "InsertCustomer Created" << std::endl;
}

InsertCustomer::InsertCustomer(Customer& cust) :  customer(cust){ 
  CopyToBuffer(phone.input.buffer, 
                customer.phone.c_str(), 
                phone.input.valid, 
                [&]() { return LenValidation(phone.input.buffer, 8); });
  CopyToBuffer(name.buffer, 
               customer.name.c_str(), 
               name.valid, [&]() { return LenValidation(name.buffer, 3); });
  CopyToBuffer(surname.buffer, 
               customer.surname.c_str(), 
               surname.valid, [&]() { return LenValidation(surname.buffer, 3); });
  CopyToBuffer(email.buffer,
               customer.email.c_str(),
               email.valid, [&]() { return IsEmailValid(email.buffer); });
}


InsertCustomer::~InsertCustomer() {
  std::cout << "Insert Customer destroyed" << std::endl;
}

void InsertCustomer::Render() {
  //if (ImGui::BeginMenuBar()) {
  //  if (ImGui::BeginMenu("Options")) {
  //    if (ImGui::MenuItem("Fields")) {

  //    }
  //    ImGui::EndMenu();
  //  }
  //  ImGui::EndMenuBar();
  //}
  FieldsSection();
  ImGui::Text(validation_feedback.c_str());
  UpdateValidationMsg();
  InsertCustButton();
  ResetOnEmptyMain();
}

void InsertCustomer::ResetOnEmptyMain() {
  if (strlen(phone.input.buffer) == 0 && !phone_field_empty) {
    name = HintInputField();
    surname = HintInputField();
    email = HintInputField();
    phone_field_empty = true;
  }
  if (strlen(phone.input.buffer) > 0) { phone_field_empty = false; }
}

void InsertCustomer::FieldsSection() {
  ImGui::SeparatorDecorator("CUSTOMER: ", FieldsValidated());
  PhoneFieldSection();
  ImGui::InputTextWithHintExt("##Name", "Name...", name, 
                              [&]() { return LenValidation(name.buffer, 3); },
                              &feedback); 
  ImGui::InputTextWithHintExt("##Surname", "Surname...", surname, 
                              [&]() { return LenValidation(surname.buffer, 3); },
                              &feedback);
  ImGui::InputTextWithHintExt("##Email", "Email...", email, 
                              [&]() {return IsEmailValid(email.buffer); }, 
                              &feedback);

  //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Debugging
  //ImGui::Text(phone.input.valid ? "true" : "false");
  //ImGui::SameLine(); ImGui::Text(name.valid ? "true" : "false");
  //ImGui::SameLine(); ImGui::Text(surname.valid ? "true" : "false");
  //ImGui::SameLine(); ImGui::Text(email.valid ? "true" : "false");
  //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

}
void InsertCustomer::PhoneFieldSection() {
  CustomerSelectedOnPopup();
  ImGui::InputTextWithPopup("##Phone", "Phone number...", phone,
                            [&]() { return LenValidation(phone.input.buffer, 8); },
                            &selected, nullptr, &feedback);
}

void InsertCustomer::CustomerSelectedOnPopup() {
  if (phone.input.valid) {
    if (selected) {
      temp_customer = Database::QueryCustomerByPhone(phone.input.buffer);
      if (temp_customer != nullptr) {
        CopyToBuffer(name.buffer, temp_customer->name.c_str(), 
                      name.valid, 
                      [&]() { return LenValidation(name.buffer, 3); });
        CopyToBuffer(surname.buffer, 
                      temp_customer->surname.c_str(), surname.valid, 
                      [&]() { return LenValidation(surname.buffer, 3); });
        CopyToBuffer(email.buffer, temp_customer->email.c_str(), 
                      email.valid, 
                      [&]() { return IsEmailValid(email.buffer); });
      }
      selected = false;
    }
  }
}

void InsertCustomer::InsertCustButton() {
  //static Customer customer;
  if (!FieldsValidated()) {
    ImGui::BeginDisabled(true);
  }
  if (ImGui::Button("Submit Customer Details")) {
    customer = InitCustomer();
    InitModal();        
  }
  if (!FieldsValidated()) {
    ImGui::EndDisabled();
  }
  RunModal(customer);
}

bool InsertCustomer::IsEmailValid(std::string buffer) {
  size_t atPos = buffer.find('@');
  size_t dotPos = buffer.find('.', atPos + 1);
  return (atPos != std::string::npos && dotPos != std::string::npos);
}

bool InsertCustomer::LenValidation(const char* buffer, int length) {
  return (strlen(buffer) >= length);
}

bool InsertCustomer::FieldsValidated() {
  return (phone.input.valid && name.valid && surname.valid 
          && email.valid);
}

int InsertCustomer::SetValidaitonErr() {
  std::vector<HintInputField> validations = { phone.input, name, surname, email };
  if (FieldsValidated())
     return -1;
  for (int i = 0; i < validations.size();  i++) {
    if (strlen(validations[i].buffer) > 0 && !validations[i].valid)
    {
        return i;
    }
  }
  return -1;
}

void InsertCustomer::UpdateValidationMsg() {
  static std::map<int, std::string> validation_error_messages = {
  {0, "Wrong phone!"},
  {1, "Wrong name!"},
  {2, "Wrong surname!"},
  {3, "Wrong email!"},
  };
  if (feedback) {
    int error_code = SetValidaitonErr();
    if (error_code != -1) {
        validation_feedback = validation_error_messages[error_code];
    }
    else {
        validation_feedback = "";
    }
    feedback = false;
  }
}

Customer InsertCustomer::InitCustomer() {
  Customer customer(phone.input.buffer, name.buffer, surname.buffer, 
                    email.buffer);
  return customer;
}

void InsertCustomer::InitModal() {
  ModalController::RenderModal("ModalConfirm Customer Details");
}

void InsertCustomer::RunModal(Customer& customer) {
  ModalController::ModalConfirm("ModalConfirm Customer Details", customer, result);
  if (result == ConfirmResult::CONIFRM_SUBMIT) {
    int customerID = Database::QueryCustomerIDByPhone(phone.input.buffer);
    if (customerID == 0) {
      Database::InsertCustomer(customer, nullptr);
      result = ConfirmResult::CONIFRM_IDLE;
      ResetFields();
    }
  }
}

void InsertCustomer::ResetFields() {
  phone.input = HintInputField({ ImGuiInputTextFlags_CharsDecimal });
  name = HintInputField();
  surname = HintInputField();
  email = HintInputField();
}

void InsertCustomer::CopyToBuffer(char* buffer, std::string source, 
                                  bool& field_validation, 
                                  std::function<bool()> validation_function) {
  std::cout << "Source: "  << source << std::endl;
  strcpy(buffer, source.c_str());
  field_validation = validation_function();
}