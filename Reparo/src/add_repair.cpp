#include "add_repair.h"
#include <iostream> // delete later
using namespace std;
char searchQuery[128] = "";

char repair_name[128] = "";
char repair_surname[128] = "";
char repair_email[128] = "";
char repair_phone[128] = "";

Part part;
int previos_model_id = 0;
PartsStockWindow stock;


InputField search_field = { "##Search", "Search for customer...", searchQuery, IM_ARRAYSIZE(searchQuery), ImGuiInputTextFlags_None };

std::vector<InputField> repair_fields = {
{"##PhoneNumber", "Phone Number..", repair_phone, IM_ARRAYSIZE(repair_phone), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank},
{"##Name", "Name..", repair_name, IM_ARRAYSIZE(repair_name), ImGuiInputTextFlags_None},
{"##Surname", "Surname..", repair_surname, IM_ARRAYSIZE(repair_surname), ImGuiInputTextFlags_None},
{"##Email", "Email..", repair_email, IM_ARRAYSIZE(repair_email), ImGuiInputTextFlags_None},
};

void AddRepair::AddRepairWindow() {
    ImGui::Begin("Add Repair");
    
    if (ImGui::BeginTable("Add repair table", 2)) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::PushItemWidth(-1);
        ImGui::SeparatorText("CUSTOMER:");

        for (int i = 0; i < repair_fields.size() ; ++i) {
            ImGui::InputTextWithHint(repair_fields[i].label, repair_fields[i].hint, repair_fields[i].buffer, repair_fields[i].bufferSize, repair_fields[i].flags);
        }

        ImGui::TableNextColumn();
        ImGui::InputTextWithHint(search_field.label, search_field.hint, search_field.buffer, search_field.bufferSize, search_field.flags);
        SearchForCustomers();
        SearchForByPhone();



        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::SeparatorText("DEVICE:");

        ComboModels();
 
        ComboCategories();
        if (part.model.current_id >= 0) {
            PartsStockWindow stock;
            stock.ResetOnModelChange(part.model, part.color, previos_model_id);
            ComboColors();
        }
        else {
            ImGui::Text("Choose model to show colors.");
        }
        ImGui::TableNextColumn();

        ImGui::EndTable();
    }
    ImGui::End();
}

void SearchForCustomers() {
    Search search;
    search.SearchField(search_field.buffer);
    search.ForAdd(repair_fields);
}
void SearchForByPhone() {
    Search search;
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Looks like this customer already exists. \nDouble click on him to copy data over.");
    search.SearchField(repair_fields[0].buffer);
    search.ForAdd(repair_fields);
}

void ComboModels() {
 
    if (!part.model.retreived){
        std::cout << "COMBO " << std::endl;
        stock.GetModels(part.model.data);
        part.model.current_id = 0;
        part.model.retreived = true;
      
    }
    if (ImGui::BeginCombo("##Models", part.model.data[part.model.current_id].c_str()))
    {
        for (int n = 0; n < part.model.data.size(); n++)
        {
            const bool is_selected = (part.model.current_id == n);
            if (ImGui::Selectable(part.model.data[n].c_str(), is_selected))
                part.model.current_id = n;
         
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void ComboCategories() {

    if (!part.category.retreived) {
        std::cout << "COMBO " << std::endl;
        stock.GetCategories(part.category.data);
        part.category.current_id = 0;
        part.category.retreived = true;


    }
    if (ImGui::BeginCombo("##Categories", part.category.data[part.category.current_id].c_str()))
    {
        for (int n = 0; n < part.category.data.size(); n++)
        {
            const bool is_selected = (part.category.current_id == n);
            if (ImGui::Selectable(part.category.data[n].c_str(), is_selected))
                part.category.current_id = n ;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void ComboColors() {

    if (!part.color.retreived) {
        std::cout << "COMBO " << std::endl;
        stock.GetColorsForModel(part.color.data,part.model.data, part.model.current_id);
        part.color.current_id = 0;
        part.color.retreived = true;
    }
    if (ImGui::BeginCombo("##Colors", part.color.data[part.color.current_id].c_str()))
    {
        for (int n = 0; n < part.color.data.size(); n++)
        {
            const bool is_selected = (part.color.current_id == n);
            if (ImGui::Selectable(part.color.data[n].c_str(), is_selected))
                part.color.current_id = n;
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}