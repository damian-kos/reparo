#include "customer_input_window.h"

char name[128] = "";
char surname[128] = "";
char email[128] = "";
char phoneNumber[128] = "";
int phoneNumberIndex = -1; // Initialize to an invalid index
std::string errorMessage = "";


using namespace std;


void CustomerInputWindow::Render(CustomerInputFlags flags) {
        
        ImGui::PushItemWidth(-1);
        CreateInputFields(flags);
        ImGui::Spacing();
        if (flags & CustomerInputFlags_SubmitButton) {
            if (ImGui::Button("Submit Customer Details")) {
                Submit();
            }
            if (TestSubmitCall("Customer Input Feedback")) {
                int var = sql.InsertCustomer(customer);
            }
        }
        //modalController.GetErrorState("Customer Input Feedback", errorMessage.c_str());
        ImGui::Spacing();
        if (flags & CustomerInputFlags_SearchResultsOnPhoneNo) 
            PassSearchQuery();
     
}

void CustomerInputWindow::CreateInputFields(CustomerInputFlags flags)
{
    // Create input fields
    for (int i = 0; i < inputFields.size(); i++) {
        if (flags & CustomerInputFlags_NoSurnameField) {
    
            if (i == 2) {
                continue;
            }
        }
  
        ImGui::InputTextWithHint(inputFields[i].label, inputFields[i].hint, inputFields[i].buffer, inputFields[i].bufferSize, inputFields[i].flags);
        if (std::strcmp(inputFields[i].label, "##PhoneNumber") == 0) {
            phoneNumberIndex = i; // Store the index of the phoneNumber field
        }
    }
}

Customer CustomerInputWindow::FieldsToCustomer() {
	customer.phone_number = inputFields[0].buffer;
	customer.name = inputFields[1].buffer;
	customer.surname = inputFields[2].buffer;
	customer.email = inputFields[3].buffer;
    return customer;
}

void CustomerInputWindow::Submit() {
    Customer curr_customer = FieldsToCustomer();
    submit_customer_result = sql.SearchForCustomerSQL(curr_customer);
}

bool CustomerInputWindow::TestSubmitCall(std::string popup_title, CustomerSubmissionFlags flags) {
    if (submit_customer_result == PhoneNumberIsEmpty) {
        modalController.RenderErrorModal(popup_title.c_str());
        std::string test = "Phone number can't be empty. It is used to identify customers.";
        modalController.GetErrorState(popup_title.c_str(), test.c_str(), submit_customer_result);
        return false;
    }
    if (submit_customer_result == AddNewCustomer) {
        for (InputField& field : inputFields) {
            memset(field.buffer, 0, field.bufferSize); // Set all characters to null (clear the buffer)
        }
        return true;
    }
    if (submit_customer_result == WrongQuery) {
        modalController.RenderErrorModal(popup_title.c_str());
        errorMessage = "There is a problem with inputting customers. Please contact support team.";
        return false;
    }
    if (flags & CustomerSubmissionFlags_None | CustomerSubmissionFlags_SimpleAdd) {
        modalController.RenderErrorModal(popup_title.c_str());
        errorMessage = "Customer with this phone number already exists. \nYou can edit his details. Right click on customer in table view and click a button.";
        return false;
    }
    if (flags & CustomerSubmissionFlags_RepairAdd ) {
        modalController.RenderErrorModal(popup_title.c_str());
        errorMessage = "Customer with this phone number already exists. \nYou can edit his details. Right click on customer in table view and click a button.";
        return true;
    }
}


void CustomerInputWindow::PassSearchQuery()
{
    Search search;

    if (phoneNumberIndex != -1) {
        search.SearchField(inputFields[phoneNumberIndex].buffer);
    }
    search.ForAdd(inputFields, SearchFlags_EditCustomer);
}



