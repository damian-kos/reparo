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

void ImGuiHelper::PartTableStockWindow(Part& part) {
    if (ImGui::BeginTable("To add", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("Brand");
        ImGui::TableSetupColumn("Model");
        ImGui::TableSetupColumn("Category");
        ImGui::TableSetupColumn("Color(if any)");
        ImGui::TableSetupColumn("Quality(if any)");
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (part.brand.current_id != -1) {
            ImGui::Text(part.brand.data[part.brand.current_id].c_str());
        }
        ImGui::TableSetColumnIndex(1);
        if (part.model.current_id != -1) {
            ImGui::Text(part.model.data[part.model.current_id].c_str());
        }
        ImGui::TableSetColumnIndex(2);
        if (part.category.current_id != -1) {
            ImGui::Text(part.category.data[part.category.current_id].c_str());
        }
        ImGui::TableSetColumnIndex(3);
        if (part.color.current_id != -1) {
            ImGui::Text(part.color.data[part.color.current_id].c_str());
        }
        ImGui::TableSetColumnIndex(4);
        if (part.quality.order.size() == 0) {
            part.quality.desc = "";
        }
        else {
            part.quality.desc = "";

            for (size_t i = 0; i < part.quality.order.size(); i++)
            {
                part.quality.desc += part.quality.data[part.quality.order[i]] + " ";
            }
        }
        ImGui::Text(part.quality.desc.c_str());
        ImGui::EndTable();
    }
}