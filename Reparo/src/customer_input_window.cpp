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


std::vector<InputField> inputFields = {
    {"##PhoneNumber", "Phone Number..", phoneNumber, IM_ARRAYSIZE(phoneNumber), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank},
    {"##Name", "Name..", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_None},
    {"##Surname", "Surname..", surname, IM_ARRAYSIZE(surname), ImGuiInputTextFlags_None},
    {"##Email", "Email..", email, IM_ARRAYSIZE(email), ImGuiInputTextFlags_None},
};


void CustomerInputWindow::Render() {
        ImGui::Begin("Add customer");
        ImGui::PushItemWidth(-1);
        CreateInputFields();
        ImGui::Spacing();
        if(ImGui::Button("Submit Customer Details"))
            Submit();
        modalController.GetErrorState("Customer Input Feedback", errorMessage.c_str());
        ImGui::Spacing();
        PassSearchQuery();
        ImGui::End();
     
}

void CustomerInputWindow::CreateInputFields()
{
    // Create input fields
    for (int i = 0; i < inputFields.size(); ++i) {
        ImGui::InputTextWithHint(inputFields[i].label, inputFields[i].hint, inputFields[i].buffer, inputFields[i].bufferSize, inputFields[i].flags);
        if (std::strcmp(inputFields[i].label, "##PhoneNumber") == 0) {
            phoneNumberIndex = i; // Store the index of the phoneNumber field
        }
    }
}

void CustomerInputWindow::Submit()
{
    CustomerPopulate populate;
    int submit = populate.Submit(inputFields, customer);

    if (submit == -2){
        modalController.RenderErrorModal("Customer Input Feedback");
        errorMessage = "Phone number can't be empty. It is used to identify customers.";
        return;
     }
    if (submit == 0) {
        int placeholder = sql.InsertCustomer(customer);
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



