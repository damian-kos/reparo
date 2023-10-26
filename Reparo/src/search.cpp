#include "search.h"
#include <iostream>
#include <unordered_map>
#include <cstring>



size_t previousLen = 0;
int previousModelLen = 0;
bool retreived = false;
bool model_retreived = false;
int selected_model = -1;

std::unordered_map<int, Customer> customers;

std::vector<std::string> found_models;

int currentlySelectedRow = -1;

void Search::SearchField(const char* searchQuery) {
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
}


void Search::ForAdd(std::vector<InputField>& fields, int label_int, SearchFlags reparo_flags) {
    CustomerEditWindow customerEditWindow;
    ImGuiHelper imguiHelper;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    
    static bool selected[10] = {};
    //if (searchResultsBox) {
    std::vector<std::string> names = { "ID", "Name", "Phone", "Email" };
    static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;


    if (imguiHelper.TableBegin("Customer", 4, names, label_int, flags)) {
        int index = 0;
        for (auto& [key, val] : customers) {
            char label[32];

            //ImGui::PushID(index);
            sprintf_s(label, "%d", index + 1); // Format as 5-digit string with leading zeros

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (ImGui::Selectable(label, selected[index], ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap)) {
                if (ImGui::IsMouseDoubleClicked(0)) {
                    for (int i = 0; i < 10; i++) {
                        selected[i] = false;
                    }
                    selected[index] = !selected[index];

                    if (reparo_flags & SearchFlags_CopyToFields) {
                        CustomerPopulate populate;
                        populate.PopulteCustomerFields(fields, val);
                        recently_populated = true;

                    }

                    currentlySelectedRow = index;
                }
            }
            if (ImGui::BeginPopupContextItem()) {
                ImGui::Text(("Customer: \"%s\".", val.name.c_str()));
                if (reparo_flags & SearchFlags_EditCustomer) {
                    std::cout << val.name << std::endl;
                    if (ImGui::SmallButton("Edit")) {
                        customerEditWindow.SetCustomerToEdit(&val, key);
                    }
                }
                if (reparo_flags & SearchFlags_CopyToFields) {
                    if (ImGui::Button("Copy Over ")) {
                        CustomerPopulate populate;
                        populate.PopulteCustomerFields(fields, val);
                        recently_populated = true;
                    }
                }
                if (ImGui::Button("Close")) {
                    ImGui::CloseCurrentPopup();
                }
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
            //ImGui::PopID();
        }
        
            ImGui::EndTable();

    }

    customers.clear();
    ImGui::PopID();
}


const char** vectorToCharArray(const std::vector<std::string>& strings) {
    const char** charArray = new const char* [strings.size()];

    for (size_t i = 0; i < strings.size(); ++i) {
        charArray[i] = strings[i].c_str();
    }
    return charArray;
}

bool Search::SearchModel(PopupInput& popup, std::vector<std::string>& vector) {
    if (popup.previous_len != strlen(popup.input)) {
        model_retreived = false;
        popup.previous_len = strlen(popup.input);
        return false;
    }
    if (strlen(popup.input) >= 1 && !model_retreived) {
        searchResultsBox = true;
        popup.previous_len = strlen(popup.input);
        return true;
    }
    else if (strlen(popup.input) < 1) {
        vector.clear();
        searchResultsBox = false;
        popup.previous_len = strlen(popup.input);
        return false;
    }
    return false;
}

void Search::PopupModels(PopupInput& input, PartData& attribute, const char* label) {
    input.is_input_text_active = ImGui::IsItemActive();
    input.is_input_text_activated = ImGui::IsItemActivated();
    const char** autocomplete = vectorToCharArray(attribute.data);
    if (ImGui::IsItemEdited()) {
        std::cout << input.input << std::endl;
    }
    if (input.is_input_text_activated) {
    
        ImGui::OpenPopup(label);
       
    }

    
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
    //ImGui::SetNextWindowSize({ ImGui::GetItemRectSize().x, 0 });
    if (ImGui::BeginPopup(label, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ChildWindow))
    {

        for (int i = 0; i < attribute.data.size(); i++)
        {
/*            if (strstr(autocomplete[i], input.input) == NULL)
                continue;*/
            if (ImGui::Selectable(autocomplete[i]))
            {
                //ImGui::ClearActiveID();
                strcpy(input.input, autocomplete[i]);
                attribute.name = attribute.data[i];
                attribute.current_id = 1;
            }
        }
        if (input.is_input_enter_pressed || (!input.is_input_text_active && !ImGui::IsWindowFocused()))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
    }
