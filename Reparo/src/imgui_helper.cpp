#include "imgui_helper.h"

void ImGuiHelper::PopulateListBox(const char* label, std::vector<std::string>& vector, int& selectable) {
    if (ImGui::BeginListBox(label)) {
        for (int n = 0; n < vector.size(); n++) {
            const bool is_selected = (selectable == n);
            if (ImGui::Selectable(vector[n].c_str(), is_selected))
                selectable = n;
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
    ImGui::SameLine();
}

void ImGuiHelper::PopulateListBoxMulti(const char* label, PartQualityData& quality) {
    if (ImGui::BeginListBox(label)) {
        for (int n = 0; n < quality.data.size(); n++) {
            if (ImGui::Selectable(quality.data[n].c_str(), quality.selections[n])) { // Removed .c_str()
                quality.selections[n] = !quality.selections[n];
                if (quality.selections[n]) {
                    quality.order.push_back(n);
                }
                else {
                    auto it = std::find(quality.order.begin(), quality.order.end(), n);
                    // Check if the index was found before proceeding
                    if (it != quality.order.end()) {
                        // Erase the index from the selected list
                        quality.order.erase(it);
                        //std::cout << "ERASED " << it._Unwrapped() << std::endl;
                    }
                }
            }
        }
        ImGui::EndListBox();
    }
    ImGui::SameLine();
}