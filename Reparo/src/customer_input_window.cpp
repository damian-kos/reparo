#include "customer_input_window.h"

char name[128] = "";
char surname[128] = "";
char email[128] = "";
char phoneNumber[128] = "";
int phoneNumberIndex = -1; // Initialize to an invalid index
std::string errorMessage = "";

SQLQuery sql;
Search search;
using namespace std;


void CustomerInputWindow::Render(CustomerInputFlags flags) {
        
        ImGui::PushItemWidth(-1);
        CreateInputFields();
        ImGui::Spacing();
        if (flags & CustomerInputFlags_SubmitButton) {
            if (ImGui::Button("Submit Customer Details")) {
                Submit();
            }
        }
        modalController.GetErrorState("Customer Input Feedback", errorMessage.c_str());
        ImGui::Spacing();
        if (flags & CustomerInputFlags_SearchResultsOnPhoneNo) 
            PassSearchQuery();
     
}

void CustomerInputWindow::CreateInputFields()
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

void CustomerInputWindow::Submit()
{
    CustomerPopulate populate;
    Customer curr_customer = FieldsToCustomer();
    int submit = populate.Submit(curr_customer);

    if (submit == -2){
        modalController.RenderErrorModal("Customer Input Feedback");
        errorMessage = "Phone number can't be empty. It is used to identify customers.";
        return;
     }
    if (submit == 0) {
        int placeholder = sql.InsertCustomer(curr_customer);
        for (InputField& field : inputFields) {
            memset(field.buffer, 0, field.bufferSize); // Set all characters to null (clear the buffer)
        }
        return;
    }
    if (submit == -1) {
        modalController.RenderErrorModal("Customer Input Feedback");
        errorMessage = "There is a problem with inputting customers. Please contact support team.";
        return;

    }
    else {
        modalController.RenderErrorModal("Customer Input Feedback");
        errorMessage = "Customer with this phone number already exists. \nYou can edit his details. Right click on customer in table view and click a button.";
        return;

    }
}


void CustomerInputWindow::PassSearchQuery()
{
    if (phoneNumberIndex != -1) {
        search.SearchField(inputFields[phoneNumberIndex].buffer);
    }
    search.CustomerTableWEdit();
}



