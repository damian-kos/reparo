#include "error_handler.h"
#include <iostream>
bool errorPresent = false;

void ModalController::RenderErrorModal(const char* error_name) {
    ImGui::OpenPopup(error_name);

}

void AppearOnCenterWindow() {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void ModalController::GetErrorState(const char* error_name, const char* errorMessage, int& state ) {
    //Always center this window when appearing
    AppearOnCenterWindow();

    if (ImGui::BeginPopupModal(error_name, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(errorMessage);
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup();  state = NotSubmitted; }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); state = NotSubmitted; }
        ImGui::EndPopup();
    }
}

void ModalController::RepairConfirmation(const char* error_name, Repair& repair, RepairSubmissionState& state) {
    AppearOnCenterWindow();
    if (ImGui::BeginPopupModal(error_name)) {
        CustomerDetails(repair.customer);
        RepairDetails(repair);

        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup();  state =  RepairSubmission_Submit; }

        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); state =  RepairSubmission_Cancel;  }
        ImGui::SetItemDefaultFocus();

        ImGui::EndPopup();
    }
}

void ModalController::RepairDetails(Repair& repair)
{
    ImGui::Text("Model: "); ImGui::SameLine(); ImGui::Text(repair.model.name.c_str());
    ImGui::Text("Color: "); ImGui::SameLine(); ImGui::Text(repair.color.name.c_str());
    ImGui::Text("Category: "); ImGui::SameLine(); ImGui::Text(repair.category.name.c_str());
    ImGui::Text("Note for customer: "); ImGui::SameLine(); ImGui::Text(repair.note.c_str());
    ImGui::Text("Note hidden: "); ImGui::SameLine(); ImGui::Text(repair.note_hidden.c_str());

    std::string double_price = std::to_string(repair.price);
    char price_buffer[32];
    sprintf(price_buffer, "%0.2f", repair.price);
    ImGui::Text("Price: "); ImGui::SameLine(); ImGui::Text(price_buffer);
}

void ModalController::CustomerDetails(Customer& customer)
{
    ImGui::Text("Name: "); ImGui::SameLine(); ImGui::Text(customer.name.c_str());
    ImGui::Text("Surname: "); ImGui::SameLine(); ImGui::Text(customer.surname.c_str());
    ImGui::Text("Phone: "); ImGui::SameLine(); ImGui::Text(customer.phone_number.c_str());
    ImGui::Text("Email: "); ImGui::SameLine(); ImGui::Text(customer.email.c_str());
}
