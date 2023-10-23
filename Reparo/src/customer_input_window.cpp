#include "customer_input_window.h"


void CustomerInputWindow::Render(CustomerInputFlags reparo_flags) {
        ImGui::PushItemWidth(-1);
        decorator.SetTestValue(test_bool);
        decorator.DecoratedSeparatorText("CUSTOMER:");
        CreateInputFields(reparo_flags);
        ImGui::Spacing();
        if (reparo_flags & CustomerInputFlags_SubmitButton) {
            if (ImGui::Button("Submit Customer Details")) {
                Submit();
                if (TestSubmitCall()) {
                    int var = sql.InsertCustomer(customer);
                }
            }
        }
        modalController.GetErrorState(error_title.c_str(), error_message.c_str(), submit_customer_result);
        ImGui::Spacing();

        if (reparo_flags & CustomerInputFlags_SearchResultsOnPhoneNo) {
            PassSearchQuery();
        }
}

void InputValidation(InputField& input) {
    std::string label = input.buffer;
    if (input.label == "##PhoneNumber") {
        if (strlen(input.buffer) > 8) {
            input.is_valid = true;
            std::cout << input.label << input.is_valid << std::endl;
        }
        else {
            input.is_valid = false;
            std::cout << input.label << input.is_valid << std::endl;
        }
    }
    if (input.label == "##Name") {
        if (strlen(input.buffer) > 3) {
            input.is_valid = true;
            std::cout << input.label << input.is_valid << std::endl;
        }
        else {
            input.is_valid = false;
            std::cout << input.label << input.is_valid << std::endl;
        }
    }
    if (input.label == "##Surname") {
        input.is_valid = true;
        std::cout << input.label  << input.is_valid  << std::endl;
    }

    if (input.label == "##Email") {
        input.is_valid = true;
        std::cout << input.label << input.is_valid << std::endl;
    }
}


void CustomerInputWindow::CreateInputFields(CustomerInputFlags reparo_flags)
{
    // Create input fields
    for (int i = 0; i < inputFields.size(); i++) {

        if (reparo_flags & CustomerInputFlags_NoSurnameField) {
            if (i == 2) { continue; }
        }

        ImGui::InputTextWithHint(inputFields[i].label, inputFields[i].hint, inputFields[i].buffer, inputFields[i].bufferSize, inputFields[i].flags);
       
        if (std::strcmp(inputFields[i].label, "##PhoneNumber") == 0) {
            phoneNumberIndex = i; // Store the index of the phoneNumber field
        }
        if (ImGui::IsItemDeactivated()) {
            InputValidation(inputFields[i]);
            std::cout << inputFields[i].buffer << std::endl;
  
        }
     
    }
    for (int i = 0; i < inputFields.size(); i++) {
        if (inputFields[i].is_valid == false) {
            test_bool = false;
            break;
        }
        else {
            test_bool = true;
        }
    }


}

Customer CustomerInputWindow::FieldsToCustomer(CustomerInputFlags reparo_flags) {
    const bool surname = (reparo_flags & CustomerInputFlags_NoSurnameField) != 0;
	customer.phone_number = inputFields[0].buffer;
	customer.name = inputFields[1].buffer;
    if (!surname) {
        customer.surname = inputFields[2].buffer;
    }
	customer.email = inputFields[3].buffer;
    return customer;
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
    if (!(reparo_flags ) || (reparo_flags & (CustomerSubmissionFlags_None | CustomerSubmissionFlags_SimpleAdd)))
    {
        std::cout << "simple add: " << CustomerSubmissionFlags_SimpleAdd << std::endl;
        modalController.RenderErrorModal(error_title.c_str());
        error_message = "Customer with this phone number already exists. \nYou can edit his details. Right click on customer in table view and click a button.";
        return false;
    }
    if (reparo_flags & CustomerSubmissionFlags_RepairAdd ) {
        std::cout << "repair add" << std::endl;
        modalController.RenderErrorModal(error_title.c_str());
        error_message = "Repair couldn't be added for unknown reasons, try again later.";
        return true;
    }
    else {
        std::cout << "nothing happened" << std::endl;
        return false;
    }
    return false;
}

void CustomerInputWindow::PassSearchQuery()
{
    Search search;
    search.SearchField(inputFields[0].buffer);  
    search.ForAdd(inputFields, 1);
}




