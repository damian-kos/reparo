#include "search.h"
#include <iostream>
#include <unordered_map>

bool searchResultsBox = false;

SQLQuery sqlSearch;
int previousLen = 0;
bool retreived = false;
std::unordered_map<int, Customer> customers;
int currentlySelectedRow = -1;



void Search() {
    CustomerEditWindow customerEditWindow;
    ImGuiHelper imguiHelper;
    static bool selected[10] = {};
    if (searchResultsBox) {
        std::vector<std::string> names = { "ID", "Name", "Phone", "Email" };

        static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
        imguiHelper.TableBegin("Customers", 4, names, flags);
            int index = 0;
            for (auto& [key, val] : customers){
                char label[32];

                ImGui::PushID(index);
                sprintf_s(label, "%d", index+1); // Format as 5-digit string with leading zeros

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                if (ImGui::Selectable(label, selected[index], ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap)) {
                    if (ImGui::IsMouseDoubleClicked(0)) {
                        for (int i = 0; i < 10; i++) {
                            selected[i] = false;
                        }
                        selected[index] = !selected[index];
                        //customerEditWindow.SetCustomerToEdit(&val, key); //To be replaced with customer details, repair history etc.
                        currentlySelectedRow = index;
                    }
                }
                imguiHelper.PopupOnItemOfTable(("Customer: \"%s\".", val.name.c_str()), val, key);
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


void SearchField(const char* searchQuery) {
    if (previousLen != strlen(searchQuery)) {
        retreived = false;
    }
    if (strlen(searchQuery) >= 3 && !retreived) {
        searchResultsBox = true;
        previousLen = strlen(searchQuery);
        std::string strSearchQuery(searchQuery);
        sqlSearch.MatchingCustomers(strSearchQuery, customers);

    }
    else if(strlen(searchQuery) < 3) {
        searchResultsBox = false;
    }
    Search();
}

