#include "insert_customer.h"

InsertCustomer::InsertCustomer() : imgui_decorator() { 
    std::cout << "InsertCustomer Created" << std::endl;
    }

InsertCustomer::InsertCustomer(Customer& cust) : imgui_decorator(), customer(cust){ 
    CopyToBuffer(phone.input.buffer, 
        customer.phone.c_str(), 
        phone.input.validated, 
        [&]() { return SimpleValidation(phone.input.buffer, 8); });
    CopyToBuffer(name.buffer, 
        customer.name.c_str(), 
        name.validated, [&]() { return SimpleValidation(name.buffer, 3); });
    CopyToBuffer(surname.buffer, 
        customer.surname.c_str(), 
        surname.validated, [&]() { return SimpleValidation(surname.buffer, 3); });
    CopyToBuffer(email.buffer,
        customer.email.c_str(),
        email.validated, [&]() { return IsEmailValid(email.buffer); });
}

InsertCustomer::~InsertCustomer() {
    std::cout << "Insert Customer destroyed" << std::endl;
}

void InsertCustomer::Render() {
    FieldsSection();
    ImGui::Text(validation_feedback.c_str());
    SubmitButton();
}

void InsertCustomer::FieldsSection() {
    imgui_decorator.SetTestValue(FieldsValidated());
    imgui_decorator.DecorateSeparatorText("CUSTOMER: ");
    CustomerSelectedOnPopup();
    CreateInputField("##Phone", "Phone number...", phone, [&]() { return SimpleValidation(phone.input.buffer, 8); });
    PopupFields("##Phone", phone);
    modals.PopupOnInputField(phone, "phone", selected);
    CreateInputField("##Name", "Name...", name, [&]() { return SimpleValidation(name.buffer, 3); });
    CreateInputField("##Surname", "Surname...", surname, [&]() { return SimpleValidation(surname.buffer, 3); });
    CreateInputField("##Email", "Email...", email, [&]() {return IsEmailValid(email.buffer); });

    //Debugging
    ImGui::Text(phone.input.validated ? "true" : "false");
    ImGui::SameLine(); ImGui::Text(name.validated ? "true" : "false");
    ImGui::SameLine(); ImGui::Text(surname.validated ? "true" : "false");
    ImGui::SameLine(); ImGui::Text(email.validated ? "true" : "false");
    //
}

void InsertCustomer::CreateInputField(const char* label, const char* hint, HintInputField& field, std::function<bool()> validation_function) {
    if (field.is_on) {
        ImGui::InputTextWithHint(label, hint, field.buffer, 128, field.imgui_flags);
        if (ImGui::IsItemActive()) {
            field.validated = validation_function();
        }
        if (ImGui::IsItemDeactivated()) {
            UpdateValidationMsg();
        }
    }
}

void InsertCustomer::CustomerAlreadyExists() {
    if (phone.input.validated)
        ImGui::Text("Customer already exists");
}

void InsertCustomer::CustomerSelectedOnPopup() {
    if (phone.input.validated) {
        temp_customer = db.QueryCustomerByPhone(phone.input.buffer);
        if (selected) {
            CopyToBuffer(name.buffer, temp_customer->name.c_str(), name.validated, [&]() { return SimpleValidation(name.buffer, 3); });
            CopyToBuffer(surname.buffer, temp_customer->surname.c_str(), surname.validated, [&]() { return SimpleValidation(surname.buffer, 3); });
            CopyToBuffer(email.buffer, temp_customer->email.c_str(), email.validated, [&]() { return IsEmailValid(email.buffer); });
            selected = false;
        }
    }
        ImGui::Text((temp_customer != nullptr) ? "Customer already exists" : " ");
}

void InsertCustomer::CreateInputField(const char* label, const char* hint, HintInputFieldsW_Popup& field, std::function<bool()> validation_function) {
    if (field.input.is_on) {
        ImGui::InputTextWithHint(label, hint, field.input.buffer, 128, field.input.imgui_flags);
        field.is_input_activated = ImGui::IsItemActivated();
        if (ImGui::IsItemEdited()) {
            field.input.validated = validation_function();
        }
        
        if (ImGui::IsItemDeactivated()) {

        }
    }
}

void InsertCustomer::PopupFields(const char* label, HintInputFieldsW_Popup& field) {
    if (ImGui::IsItemEdited() || ImGui::IsItemActivated()) {
        db.ManageSearchState(label, field.attribute, field.input.buffer);
    }
}

void InsertCustomer::SubmitButton() {
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

bool InsertCustomer::SimpleValidation(const char* buffer, int length) {
    return (strlen(buffer) >= length);
}

bool InsertCustomer::FieldsValidated() {
    return (phone.input.validated && name.validated && surname.validated && email.validated);
}

int InsertCustomer::SetValidaitonErr() {
    std::vector<HintInputField> validations = { phone.input, name, surname, email };
    if (FieldsValidated())
        return -1;
    for (int i = 0; i < validations.size();  i++) {
        if (strlen(validations[i].buffer) > 0 && !validations[i].validated)
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
    int error_code = SetValidaitonErr();
    if (error_code != -1) {
        validation_feedback = validation_error_messages[error_code];
    }
    else {
        validation_feedback = "";
    }
}

Customer InsertCustomer::InitCustomer() {
    Customer customer(phone.input.buffer, name.buffer, surname.buffer, email.buffer);
    std::cout << customer.phone << std::endl;
    return customer;
}

void InsertCustomer::InitModal() {
    modals.RenderModal("Confirm Customer Details");
}

void InsertCustomer::RunModal(Customer& customer) {
   modals.SubmitConfirm("Confirm Customer Details", customer, result);
   if (result == ConfirmResult::CONIFRM_SUBMIT) {
       int customerID = db.QueryCustomerIDByPhone(phone.input.buffer);
       if (customerID == 0) {
           db.InsertCustomer(customer, nullptr);
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

void InsertCustomer::CopyToBuffer(char* buffer, std::string source, bool& field_validation, std::function<bool()> validation_function) {
    std::cout << "Source: "  << source << std::endl;
    strcpy(buffer, source.c_str());
    field_validation = validation_function();
}