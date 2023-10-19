#include "imgui_helper.h"
#include <iostream>

void ImGuiHelper::PopulateListBox(const char* label, PartData& part) {
    if (ImGui::BeginListBox(label)) {
        for (int n = 0; n < part.data.size(); n++) {
            const bool is_selected = (part.current_id == n);
            if (ImGui::Selectable(part.data[n].c_str(), is_selected)) {
                part.current_id = n;
                part.name = part.data[n].c_str();
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
            if (ImGui::Selectable(quality.data[n].c_str(), quality.selections[n])) { 
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
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    if (ImGui::BeginTable(label, columns, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 5))) {
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

void ImGuiHelper::ComboForDevice(const char* label, PartData& attribute) {
    if (ImGui::BeginCombo(label, attribute.data[attribute.current_id].c_str()))
    {
        for (int n = 0; n < attribute.data.size(); n++)
        {
            const bool is_selected = (attribute.current_id == n);
            if (ImGui::Selectable(attribute.data[n].c_str(), is_selected)) {
                attribute.current_id = n;
                attribute.name = attribute.data[n];
                std::cout << attribute.name << std::endl;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}
int poop = -1;

void ImGuiHelper::RepairStatesTable(std::vector<Repair>& repairs, int& selected) {
    //const bool item_is_selected = repairs.

    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit| ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
    if (ImGui::BeginTable("##states", 6, flags)) {
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Model", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Category", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Color", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Price", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Note", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        for (int row = 0; row < repairs.size(); row++)
        {
            const bool is_selected = (selected == row);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            char label[32];

            ImGui::PushID(row);
            sprintf_s(label, "%d", repairs[row].id); // Format as 5-digit string with leading zeros

            if (ImGui::Selectable(label, is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap)) {                
                    selected = row;
            }
   
            ImGui::TableNextColumn();
            ImGui::Text(repairs[row].model.name.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(repairs[row].category.name.c_str());
            ImGui::TableNextColumn();
            ImGui::Text(repairs[row].color.name.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%.2f", repairs[row].price);
            ImGui::TableNextColumn();
            ImGui::Text(repairs[row].note.c_str());



            ImGui::PopID();
        
        }
        ImGui::EndTable();

    }
}
