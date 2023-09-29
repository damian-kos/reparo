#include "edit_customer.h"
#include <iostream>

char editName[128] = "";
char editSurname[128] = "";
char editEmail[128] = "";
char editPhoneNumber[128] = "";

bool shouldRender = false;

InputField editFields[4] = {
    {"##Name", "Name..", editName, IM_ARRAYSIZE(editName), ImGuiInputTextFlags_None},
    {"##Surname", "Surname..", editSurname, IM_ARRAYSIZE(editSurname), ImGuiInputTextFlags_None},
    {"##Email", "Email..", editEmail, IM_ARRAYSIZE(editEmail), ImGuiInputTextFlags_None},
    {"##PhoneNumber", "Phone Number..", editPhoneNumber, IM_ARRAYSIZE(editPhoneNumber), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank}
};

json customerDataToEdit;

void CustomerEditWindow::Render() {
    if (shouldRender) {
        ImGui::Begin("Edit customer");

      

        for (int i = 0; i < sizeof(editFields) / sizeof(editFields[0]); ++i) {
            ImGui::InputTextWithHint(editFields[i].label, editFields[i].hint, editFields[i].buffer, editFields[i].bufferSize, editFields[i].flags);
        }
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
        if (ImGui::Button("Update Customer Data")){
            const int& id = customerDataToEdit["ID"];

            if (InputsHandler::HandleInputsFromFields(editFields, sizeof(editFields) / sizeof(editFields[0]), &id))  {
      
            }
            else {
                ModalController modalController;
                modalController.RenderErrorModal("Missing values");
            }
        }
        modalController.GetErrorState("Missing values", "All values in fields must be not empty!");
        ImGui::PopStyleColor(1);
        ImGui::End();
    }
}
void CustomerEditWindow::DataToFields()
{
    if (customerDataToEdit.is_object()) {
        // Extract values from the JSON object and assign them to the character arrays
        strncpy_s(editName, customerDataToEdit["Name"].get<std::string>().c_str(), sizeof(editName));
        strncpy_s(editSurname, customerDataToEdit["Surname"].get<std::string>().c_str(), sizeof(editSurname));
        strncpy_s(editEmail, customerDataToEdit["Email"].get<std::string>().c_str(), sizeof(editEmail));
        strncpy_s(editPhoneNumber, customerDataToEdit["PhoneNumber"].get<std::string>().c_str(), sizeof(editPhoneNumber));
    }
}
void CustomerEditWindow::SetCustomerToEdit(json* customerData) {
    customerDataToEdit = *customerData;
}

void CustomerEditWindow::SetShouldRender(bool setter) {
    shouldRender = !shouldRender;
}

bool CustomerEditWindow::GetShouldRender() {
    return shouldRender;
}