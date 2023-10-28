#include "customer_input_window.h"

void CustomerInputWindow::Render() {
        //ImGui::PushItemWidth(-1);
        decorator.SetTestValue(validated);
        decorator.DecoratedSeparatorText("CUSTOMER:");
        CreateInputFields(); 
        ImGui::Text(validate_feedback.c_str());

        ImGui::Spacing();
        if (reparo_flags & CustomerInputFlags_SubmitButton) {
            SubmitCustomerButton();
        }
        modalController.GetErrorState(error_title.c_str(), error_message.c_str(), submit_customer_result);
        ImGui::Spacing();
        if (reparo_flags & CustomerInputFlags_SearchResultsOnPhoneNo) {
            PassSearchQuery();
        }
}

void CustomerInputWindow::SubmitCustomerButton() {
    if (!validated) {
        ImGui::BeginDisabled(true);
    }
    if (ImGui::Button("Submit Customer Details")) {
        Submit();
        if (TestSubmitCall()) {
            int var = sql.InsertCustomer(customer);
            ValidationCheck();
        }
    }
    if (!validated) {
        ImGui::EndDisabled();
    }
}

void CustomerInputWindow::Submit() {
    Customer curr_customer = FieldsToCustomer();
    submit_customer_result = sql.SearchForCustomerSQL(curr_customer);
}

bool CustomerInputWindow::TestSubmitCall(CustomerSubmissionFlags reparo_flags) {
    if (submit_customer_result == PhoneNumberIsEmpty) {
        std::cout << "phonenumberisempty" << std::endl;
        error_message = "Phone number can't be empty. It is used to identify customers.";
        modalController.RenderErrorModal(error_title.c_str());
        return false;
    }
    if (submit_customer_result == AddNewCustomer) {
        std::cout << "add new customer" << std::endl;
        for (InputField& field : inputFields) {
            memset(field.buffer, 0, field.bufferSize); // Set all characters to null (clear the buffer)
        }
        return true;
    }
    if (submit_customer_result == WrongQuery) {
        std::cout << "wrong query" << std::endl;
        modalController.RenderErrorModal(error_title.c_str());
        error_message = "There is a problem with inputting customers. Please contact support team.";
        return false;
    }
    if (!(reparo_flags) || (reparo_flags & (CustomerSubmissionFlags_None | CustomerSubmissionFlags_SimpleAdd)))
    {
        std::cout << "simple add: " << CustomerSubmissionFlags_SimpleAdd << std::endl;
        modalController.RenderErrorModal(error_title.c_str());
        error_message = "Customer with this phone number already exists. \nYou can edit his details. Right click on customer in table view and click a button.";
        return false;
    }
    if (reparo_flags & CustomerSubmissionFlags_RepairAdd) {
        std::cout << "repair add" << std::endl;
        for (InputField& field : inputFields) {
            memset(field.buffer, 0, field.bufferSize); // Set all characters to null (clear the buffer)
        }
        InputValidation();
        ValidationCheck();
        //modalController.RenderErrorModal(error_title.c_str());
        //error_message = "Repair couldn't be added for unknown reasons, try again later.";
        return true;
    }
    else {
        std::cout << "nothing happened" << std::endl;
        return false;
    }
    return false;
}

void CustomerInputWindow::ResetFields() {
    customer = Customer();
    for (InputField& field : inputFields) {
        memset(field.buffer, 0, field.bufferSize); // Set all characters to null (clear the buffer)
    }
    InputValidation();
    ValidationCheck();
}

void CustomerInputWindow::InputValidation(InputField& input) {
    std::string label = input.buffer;
    if (input.label == "##PhoneNumber")
        input.is_valid = (strlen(input.buffer) > 8);
    if (input.label == "##Name") 
        input.is_valid = (strlen(input.buffer) > 3);
    if (input.label == "##Email") {
        std::string email = input.buffer;
        input.is_valid = IsEmailValid(email);
    }
    if (input.label == "##Surname") 
        input.is_valid = (strlen(input.buffer) > 3);
}

void CustomerInputWindow::InputValidation() {
    for (InputField& input : inputFields) {
        std::string label = input.buffer;
        if (input.label == "##PhoneNumber")
            input.is_valid = (strlen(input.buffer) > 8);
        if (input.label == "##Name")
            input.is_valid = (strlen(input.buffer) > 3);
        if (input.label == "##Email") {
            std::string email = input.buffer;
            input.is_valid = IsEmailValid(email);
        }
        if (input.label == "##Surname")
            input.is_valid = (strlen(input.buffer) > 3);
    }
}

bool CustomerInputWindow::IsEmailValid(std::string email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos + 1);

    return (atPos != std::string::npos && dotPos != std::string::npos);
}

void CustomerInputWindow::CreateInputFields() {
    for (InputField& input : inputFields) {
        if (reparo_flags & CustomerInputFlags_NoSurnameField && input.label == "##Surname") {
            continue;
        }
        ImGui::InputTextWithHint(input.label, input.hint, input.buffer, input.bufferSize, input.flags);

        if (ImGui::IsItemEdited()) {
            InputValidation(input);
            std::cout << input.label << " " << input.is_valid << std::endl;
            ValidationCheck();
        }
        if (ImGui::IsItemDeactivated()) {
            ValidationFeedback();
        }
    }
}

void CustomerInputWindow::ValidationCheck() {
    bool allFieldsValid = true;
    validate_error = -1;

    for (size_t i = 0; i < inputFields.size(); i++)
    {
        if (reparo_flags & CustomerInputFlags_NoSurnameField && inputFields[i].label == "##Surname")
            continue;
        if (!inputFields[i].is_valid) {
            allFieldsValid = false;
            if (strlen(inputFields[i].buffer) > 0) {
                validate_error = i;
                break;
            }
        }
    }
    validated = allFieldsValid;
}

Customer CustomerInputWindow::FieldsToCustomer() {
    const bool surname = (reparo_flags & CustomerInputFlags_NoSurnameField) != 0;
	customer.phone_number = inputFields[0].buffer;
	customer.name = inputFields[1].buffer;
    if (!surname) {
        customer.surname = inputFields[2].buffer;
    }
	customer.email = inputFields[3].buffer;
    return customer;
}

void CustomerInputWindow::ValidationFeedback() {
        validate_feedback = "";
    if (validate_error == 0 && strlen(inputFields[0].buffer) > 0)
        validate_feedback = "Wrong number format";
    if(validate_error == 1 && strlen(inputFields[1].buffer) > 0)
        validate_feedback = "Wrong name format";
    if (validate_error == 2)
        validate_feedback = "Wrong surname format";
    if (validate_error == 3)
        validate_feedback = "Wrong email format";
}

void CustomerInputWindow::PassSearchQuery() {
    Search search;
    search.SearchField(inputFields[0].buffer);  
    search.ForAdd(inputFields, 1);
}

bool CustomerInputWindow::GetValidationState() const {
    return validated;
}


