#include "edit_customer.h"

char editName[128] = "";

char editSurname[128] = "";
char editEmail[128] = "";
char editPhoneNumber[128] = "";
Customer customerDataToEdit;
int idToEdit;
bool shouldRender = false;

std::vector<InputField> editFields = {
    {"##PhoneNumber", "Phone Number..", editPhoneNumber, IM_ARRAYSIZE(editPhoneNumber), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank},
    {"##Name", "Name..", editName, IM_ARRAYSIZE(editName), ImGuiInputTextFlags_None},
    {"##Surname", "Surname..", editSurname, IM_ARRAYSIZE(editSurname), ImGuiInputTextFlags_None},
    {"##Email", "Email..", editEmail, IM_ARRAYSIZE(editEmail), ImGuiInputTextFlags_None},
};


void CustomerEditWindow::Render() {
    if (shouldRender) {
        ImGui::Begin("Edit customer");
        for (int i = 0; i < editFields.size(); ++i) {
            ImGui::InputTextWithHint(editFields[i].label, editFields[i].hint, editFields[i].buffer, editFields[i].bufferSize, editFields[i].flags);
        }
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
        if (ImGui::Button("Update Customer Data")){
            Customer editedCustomer;
            editedCustomer.phone_number = editFields[0].buffer;
            editedCustomer.name = editFields[1].buffer;
            editedCustomer.surname = editFields[2].buffer;
            editedCustomer.email = editFields[3].buffer;
            SQLQuery sql;
            sql.UpdateCustomer(idToEdit, editedCustomer);
        }
        //modalController.GetErrorState("Missing values", "All values in fields must be not empty!");
        ImGui::PopStyleColor(1);
        ImGui::End();
    }
}

void CustomerEditWindow::DataToFields()
{
    CustomerPopulate populate;
    populate.PopulteCustomerFields(editFields, customerDataToEdit);
}

void CustomerEditWindow::SetCustomerToEdit(Customer* customerData, int id) {
    std::cout << customerData->name << " " << id << std::endl;
    idToEdit = id;
    customerDataToEdit = *customerData;
    DataToFields();
    SetShouldRender(true);
}

void CustomerEditWindow::SetShouldRender(bool setter) {
    shouldRender = !shouldRender;
}

bool CustomerEditWindow::GetShouldRender() {
    return shouldRender;
}

