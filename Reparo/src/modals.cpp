#include "modals.h"

void ModalController::RenderModal(const char* modal_title) {
    ImGui::OpenPopup(modal_title);
}

void ModalController::CenterAlign() {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void ModalController::SubmitConfirm(const char* modal_title, Customer& customer, ConfirmResult& result) {
    CenterAlign();
    if (ImGui::BeginPopupModal(modal_title, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Do you want to insert customer with this details?");
        ImGui::Separator();
        ImGui::Text("Phone: "); ImGui::SameLine(); ImGui::Text(customer.phone.c_str());
        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup();  result = ConfirmResult::ConfirmSubmit; }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); result  = ConfirmResult::ConfirmCancel; }
        ImGui::EndPopup();
    }
}

void ModalController::SubmitConfirm(const char* modal_title, Repair& repair, ConfirmResult& result) {
    CenterAlign();
    if (ImGui::BeginPopupModal(modal_title, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Do you want to insert repair with this details?");
        ImGui::Separator();
        ImGui::Text("Phone: "); ImGui::SameLine(); ImGui::Text(repair.customer.phone.c_str());
        ImGui::Text("Model: "); ImGui::SameLine(); ImGui::Text(repair.device.name);

        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup();  result = ConfirmResult::ConfirmSubmit; }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); result = ConfirmResult::ConfirmCancel; }
        ImGui::EndPopup();
    }
}


const char** vectorToCharArray(const std::vector<std::string>& strings) {
    const char** charArray = new const char* [strings.size()];

    for (size_t i = 0; i < strings.size(); ++i) {
        charArray[i] = strings[i].c_str();
    }
    return charArray;
}

void ModalController::PopupOnInputField(HintInputFieldsW_Popup& field, const char* label) {

    field.is_input_active = ImGui::IsItemActive();
    field.is_input_activated = ImGui::IsItemActivated();

    if (field.is_input_activated) {
        ImGui::OpenPopup(label);
    }

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
    if (ImGui::BeginPopup(label, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ChildWindow))
    {
        const char** autocomplete = vectorToCharArray(field.attribute.data);
        for (int i = 0; i < field.attribute.data.size(); i++)
        {

            /*            if (strstr(autocomplete[i], input.input) == NULL)
                            continue;*/
            if (ImGui::Selectable(autocomplete[i]))
            {
                //ImGui::ClearActiveID();
                strcpy(field.input.buffer, autocomplete[i]);
                field.input.validated = true;
                field.attribute.name = field.attribute.data[i];
                //field.attribute.current_id = 1;
            }
        }
        if (field.is_input_enter_pressed || (!field.is_input_active && !ImGui::IsWindowFocused())) {
            ImGui::CloseCurrentPopup();
            //field.input.check_in_db = true;
        }
        
        ImGui::EndPopup();
        

    }
    //if (ImGui::IsItemDeactivated()) {
    //    input.check_in_db = true;
    //}

}