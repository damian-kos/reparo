#include "customer_input_window.h"

char name[128] = "";
char surname[128] = "";
char email[128] = "";
char phoneNumber[128] = "";
int phoneNumberIndex = -1; // Initialize to an invalid index

SQLQuery sql;
Search search;

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
            Submit(inputFields, customer);
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

void CustomerInputWindow::Submit(std::vector<InputField>& input, Customer& cust)
{
    // Pass filled input fields to the handler.

        cust.phone_number = input[0].buffer;
        cust.name = input[1].buffer;
        cust.surname = input[2].buffer;
        cust.email = input[3].buffer;
        int test = sql.SearchForCustomerSQL(cust);
        if (test == 0) {
            sql.InsertCustomer(cust);
            for (InputField& field : input) {
            memset(field.buffer, 0, field.bufferSize); // Set all characters to null (clear the buffer)
            }
        }
        else {
            ModalController modalController;
            modalController.RenderErrorModal("Missing values");
        }

modalController.GetErrorState("Missing values", "All fields must be filled, please try again.");

    


}


void CustomerInputWindow::PassSearchQuery()
{
    if (phoneNumberIndex != -1) {
        search.SearchField(inputFields[phoneNumberIndex].buffer);
    }

    search.ForEdit();
}



