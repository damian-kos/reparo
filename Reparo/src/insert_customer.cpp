#include "insert_customer.h"

void InsertCustomer::Render() {
    ImGui::Checkbox("Phone", &name_field);
    CreateInputField("##Phone", "Phone number...", phone, name_field);

}

void InsertCustomer::CreateInputField(const char* label, const char* hint, char* buffer, bool& field){
    if(field)
        ImGui::InputTextWithHint(label, hint, buffer, 128, ImGuiInputTextFlags_None);

}