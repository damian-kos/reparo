#include "search.h"
#include <iostream>
#include <unordered_map>

bool searchResultsBox = false;

json customerDataJson;
json matchingRecords;

SQLQuery sqlSearch;
int previousLen = 0;
bool retreived = false;
//std::vector<Customer> customers;

std::unordered_map<int, Customer> customers;

int currentlySelectedRow = -1;

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
    CustomerEditWindow customerEditWindow;
   

    static bool selected[10] = {};
    if (searchResultsBox) {

        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
        if (ImGui::BeginTable("table1", 4, flags))
        {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Phone", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Email", ImGuiTableColumnFlags_WidthFixed);
            //ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed);

            ImGui::TableHeadersRow();
            int index = 0;
            for (auto& [key, val] : customers){
                char label[32];

                ImGui::PushID(index);
                sprintf_s(label, "%d", index); // Format as 5-digit string with leading zeros

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                if (ImGui::Selectable(label, selected[index], ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap)) {
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        //ImGui::OpenPopup("popup");
                        for (int i = 0; i < 10; i++) {
                            selected[i] = false;
                        }
                        selected[index] = !selected[index];

                        customerEditWindow.SetCustomerToEdit(&val, key);
                        customerEditWindow.DataToFields();
                        customerEditWindow.SetShouldRender(true);

                        currentlySelectedRow = index;
                    }
                }

                if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
                {
                    ImGui::Text("This a popup for \"%s\"!", val.name.c_str());
                    if (ImGui::SmallButton("Edit")) {
                        customerEditWindow.SetCustomerToEdit(&val, key);
                        customerEditWindow.DataToFields();
                        customerEditWindow.SetShouldRender(true);
                    }
                    if (ImGui::Button("Close"))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                    }
                ImGui::SetItemTooltip("Right-click to open options");

                // Column 1: Name
                ImGui::TableNextColumn();
                ImGui::Text("%s", val.name.c_str());

                // Column 2: Phone
                ImGui::TableNextColumn();
                ImGui::Text("%s", val.phone_number.c_str());

                //Column 3: Email
                ImGui::TableNextColumn();
                ImGui::Text("%s", val.email.c_str());
          
                
                index++;
                ImGui::PopID();
            }

        ImGui::EndTable();
        }
    }
}

void SearchField(const char* searchQuery) {
    //ImGui::InputTextWithHint("##Search", "Search..", searchQuery, IM_ARRAYSIZE(searchQuery));
    if (previousLen != strlen(searchQuery)) {
        retreived = false;
    }
    if (strlen(searchQuery) >= 3 && !retreived) {
        searchResultsBox = true;
        previousLen = strlen(searchQuery);

        std::string strSearchQuery(searchQuery);

        sqlSearch.MatchingCustomers(strSearchQuery, customers);
      
        //retreived = true;
        //std::cout << "retreiving..." << std::endl;
    }
    else if(strlen(searchQuery) < 3) {
        searchResultsBox = false;
    }
    Search();
}

