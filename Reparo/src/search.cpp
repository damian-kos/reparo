#include "search.h"
#include <iostream>


bool searchResultsBox = false;

json customerDataJson;
json matchingRecords;

void LoadPresentCustomerData() {
    CustomerData::LoadCustomerData("customer_data.json");
    customerDataJson = CustomerData::GetCustomerData();

}

void MatchingResults(const char* search) {
    matchingRecords = json::array();

    // Iterate through each record in the JSON data
    for (const auto& record : customerDataJson) {
        // Check if the "PhoneNumber" field contains the search query
        std::string phoneNumber = record["PhoneNumber"];
        if (phoneNumber.find(search) != std::string::npos) {
            // Add the matching record to the result array
            matchingRecords.push_back(record);
        }
    }
    //std::cout << matchingRecords << std::endl;
}

void Search() {
    static bool selected[10] = {};
    //static int selected = -1;
    if (searchResultsBox) {
        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        if (ImGui::BeginTable("table1", 5, flags))
        {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Phone", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Email", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed);

            ImGui::TableHeadersRow();

            for (int row = 0; row < matchingRecords.size() ; row++)
            {
                const json& record = matchingRecords[row];

                char label[32];
                int id = record["ID"].get<int>();
                sprintf_s(label, "%05d", id); // Format as 5-digit string with leading zeros

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                if (ImGui::Selectable(label, selected[row], ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        selected[row] = !selected[row];
                        std::cout << matchingRecords[row] << std::endl;

                    }
                }

                // Column 1: Name
                ImGui::TableNextColumn();
                ImGui::Text("%s", record["Name"].get<std::string>().c_str());

                // Column 2: Phone
                ImGui::TableNextColumn();
                ImGui::Text("%s", record["PhoneNumber"].get<std::string>().c_str());

                // Column 3: Email
                ImGui::TableNextColumn();
                ImGui::Text("%s", record["Email"].get<std::string>().c_str());

                //ImGuiSelectableFlags selectable_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_None;
                
                ImGui::TableNextColumn();

                //if (ImGui::TableNextColumn())
                {
                    if (ImGui::SmallButton("Edit")) {
                        std::cout << "Edit" << std::endl            ;
                    }
                    ImGui::SameLine();
                    ImGui::Text("/");
                    ImGui::SameLine();
                    if (ImGui::SmallButton("Add repair"))
                    {
                        std::cout << "Add repair" << std::endl;
                    }
                }

            }
            ImGui::EndTable();
        }
    }
}

void SearchField(const char* searchQuery) {
    //ImGui::InputTextWithHint("##Search", "Search..", searchQuery, IM_ARRAYSIZE(searchQuery));
    if (strlen(searchQuery) >= 3) {
        std::cout << searchQuery << std::endl;
        searchResultsBox = true;
        LoadPresentCustomerData();
        MatchingResults(searchQuery);
        Search();
    }
    else {
        searchResultsBox = false;
    }
}

