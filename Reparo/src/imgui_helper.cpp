#include "imgui_helper.h"
#include <iostream>

void ImGuiHelper::PopulateListBox(const char* label, std::vector<std::string>& vector, int& selectable, std::string& text) {
    if (ImGui::BeginListBox(label)) {
        for (int n = 0; n < vector.size(); n++) {
            const bool is_selected = (selectable == n);
            if (ImGui::Selectable(vector[n].c_str(), is_selected)) {
                selectable = n;
                text = vector[n].c_str();
            }
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
    std::vector<std::string> names = { "Brand", "Model", "Category", "Color(if any)", "Quality(if any)" };
    std::vector<std::string> texts = { part.brand.name, part.model.name, part.category.name, part.color.name};
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;
    TableBegin("To add", 5, names, flags);
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text(part.brand.name.c_str());
        ImGui::TableNextColumn();
        ImGui::Text(part.model.name.c_str());
        ImGui::TableNextColumn();
        ImGui::Text(part.category.name.c_str());
        ImGui::TableNextColumn();
        ImGui::Text(part.color.name.c_str());
        
        ImGui::TableNextColumn();
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


void ImGuiHelper::TableBegin(const char* label, int columns, std::vector<std::string> columns_name, ImGuiTableFlags flags) {
    if (ImGui::BeginTable(label, columns, flags)) {
        for (int i = 0; i < columns; i++)
        {
            ImGui::TableSetupColumn(columns_name[i].c_str());
        }
        ImGui::TableHeadersRow();
    }
}

void ImGuiHelper::TablesColumnsText(int columns, std::string text) {
        ImGui::TableNextColumn();
        ImGui::Text(text.c_str());
}

void ImGuiHelper::PopupOnItemOfTable(const char* text, Customer& val, int id) {
    CustomerEditWindow customerEditWindow;

    if (ImGui::BeginPopupContextItem()) {
        ImGui::Text(text);
        if (ImGui::SmallButton("Edit")) {
            customerEditWindow.SetCustomerToEdit(&val, id);
        }
        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
    ImGui::SetItemTooltip("Right-click to open options");
}

