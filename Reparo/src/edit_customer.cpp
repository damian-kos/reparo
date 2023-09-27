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

void CustomerEditWindow::Render() {
    if (shouldRender)
        std::cout << shouldRender << std::endl;
    if (shouldRender) {
        ImGui::Begin("Edit customer");

        for (int i = 0; i < sizeof(editFields) / sizeof(editFields[0]); ++i) {
            ImGui::InputTextWithHint(editFields[i].label, editFields[i].hint, editFields[i].buffer, editFields[i].bufferSize, editFields[i].flags);
        }
        ImGui::End();
    }
}

void CustomerEditWindow::SetShouldRender(bool setter) {
    shouldRender = !shouldRender;
}

bool CustomerEditWindow::GetShouldRender() {
    return shouldRender;
}