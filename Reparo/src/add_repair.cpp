#include "add_repair.h"
#include <iostream> // delete later
using namespace std;
char searchQuery[128] = "";

char repair_name[128] = "";
char repair_surname[128] = "";
char repair_email[128] = "";
char repair_phone[128] = "";
char notes[512] = "";
char notes_hidden[512] = "";
float price = 0.00f;



//Part device;
RepairData state;
int previos_model_id = 0;
PartsStockWindow stock;
Part device;


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

        //ComboModels(device);
        Combo(device, "##Models", device.model);
        Combo(device, "##Categories", device.category);

        //ComboCategories(device);
        if (device.model.current_id >= 0) {
            PartsStockWindow stock;
            stock.ResetOnModelChange(device.model, device.color, previos_model_id);
            //ComboColors(device);
            Combo(device, "##Colors", device.color);
        }
        else {
            ImGui::Text("Choose model to show colors.");
        }

        ImGui::SeparatorText("Notes:");
        ImGui::InputTextWithHint("##Notes", "Notes visible for customer..", notes, IM_ARRAYSIZE(notes), ImGuiInputTextFlags_None);
        ImGui::InputTextWithHint("##Notes_hidden", "Notes hidden from customer..", notes_hidden, IM_ARRAYSIZE(notes_hidden), ImGuiInputTextFlags_None);

        ImGui::SeparatorText("Status:");
        ComboStates(state);

        ImGui::SeparatorText("Price:");
        ImGui::InputFloat("input float", &price, 0.01f, 1.0f, "%.2f"); 



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
void Combo(Part& device, std::string label, PartData& attribute) {
    if (!attribute.retreived) {
        if (label == "##Models") {
            stock.GetModels(device.model.data);
        }
        else if (label == "##Categories") {
            stock.GetCategories(device.category.data);
        }
        else if (label == "##Colors") {
            stock.GetColorsForModel(device.color.data, device.model.data, device.model.current_id);
        }
        attribute.current_id = 0;
        attribute.retreived = true;
    }
    ImGuiHelper helper;
    helper.ComboForDevice(label.c_str(), attribute);
}
void ComboModels(Part& device) {
 
    if (!device.model.retreived){
        stock.GetModels(device.model.data);
        device.model.current_id = 0;
        device.model.retreived = true;
      
    }
    ImGuiHelper imguiHelper;
    imguiHelper.ComboForDevice("##Models", device.model);
}

void ComboCategories(Part& device) {

    if (!device.category.retreived) {
        stock.GetCategories(device.category.data);
        device.category.current_id = 0;
        device.category.retreived = true;
    }
    ImGuiHelper imguiHelper;
    imguiHelper.ComboForDevice("##Categories", device.category);

}

void ComboColors(Part& device) {

    if (!device.color.retreived) {
        stock.GetColorsForModel(device.color.data,device.model.data, device.model.current_id);
        device.color.current_id = 0;
        device.color.retreived = true;
    }
    ImGuiHelper imguiHelper;
    imguiHelper.ComboForDevice("##Colors", device.color);
}

void ComboStates(RepairData& state) {

    if (!state.retreived) {
        stock.GetStates(state);
        state.current_id = 0;
        state.retreived = true;
    }
    if (ImGui::BeginCombo("##States", state.data[state.current_id].c_str()))
    {
        for (int n = 0; n < state.data.size(); n++)
        {
            const bool is_selected = (state.current_id == n);
            if (ImGui::Selectable(state.data[n].c_str(), is_selected))
                state.current_id = n;
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}