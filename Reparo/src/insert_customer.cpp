#include "insert_customer.h"

InsertCustomer::InsertCustomer() : imgui_decorator() { }

InsertCustomer::~InsertCustomer() {
    std::cout << "Insert Customer destroyed" << std::endl;
}

void InsertCustomer::Render() {
    imgui_decorator.SetTestValue(FieldsValidated());
    imgui_decorator.DecorateSeparatorText("CUSTOMER: ");
    CreateInputField("##Phone", "Phone number...", phone, [&]() { return SimpleValidation(phone.buffer, 8); });
    CreateInputField("##Name", "Name...", name, [&]() { return SimpleValidation(name.buffer, 3); });
    CreateInputField("##Surname", "Surname...", surname, [&]() { return SimpleValidation(surname.buffer, 3); });
    CreateInputField("##Email", "Email...", email, [&]() {return IsEmailValid(email.buffer); });
    ImGui::Text(validation_feedback.c_str());
    SubmitButton();
}

void InsertCustomer::CreateInputField(const char* label, const char* hint, HintInputField& field, std::function<bool()> validation_function) {
    if (field.is_on) {
        ImGui::InputTextWithHint(label, hint, field.buffer, 128, field.imgui_flags);
        if (ImGui::IsItemActive()) {
            field.validated = validation_function();
        }
        if (ImGui::IsItemDeactivated()) {
            SetValidationMsg();
        }
    }
}

void InsertCustomer::SubmitButton() {
    static Customer customer;
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
    return (phone.validated && name.validated && surname.validated && email.validated);
}

int InsertCustomer::SetValidaitonErr() {
    std::vector<HintInputField> validations = { phone, name, surname, email };
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

void InsertCustomer::SetValidationMsg() {
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
    Customer customer(phone.buffer, name.buffer, surname.buffer, email.buffer);
    std::cout << customer.phone << std::endl;
    return customer;
}

void InsertCustomer::InitModal() {
    modals.RenderModal("Confirm Customer Details");
}

void InsertCustomer::RunModal(Customer& customer) {
   modals.SubmitConfirm("Confirm Customer Details", customer, result);
   if (result == ConfirmResult::ConfirmSubmit) {
       int customerID = db.QueryCustomerByPhone(phone.buffer);
       if (customerID == 0) {
           db.InsertCustomer(customer);
           result = ConfirmResult::ConfirmCancel;
           ResetFields();
       }
    }
}

void InsertCustomer::ResetFields() {
    phone = HintInputField({ ImGuiInputTextFlags_CharsDecimal });
    name = HintInputField();
    surname = HintInputField();
    email = HintInputField();
}