#include "error_handler.h"

bool errorPresent = false;

void ModalController::RenderErrorModal(const char* errorName) {
    ImGui::OpenPopup(errorName);


}

void ModalController::GetErrorState(const char* errorName, const char* errorMessage) {
    //Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal(errorName, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(errorMessage);
        ImGui::Separator();

       

        //static bool dont_ask_me_next_time = false;
       /* ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
        ImGui::PopStyleVar();*/

        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
}