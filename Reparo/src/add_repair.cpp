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


Customer repair_customer;
int previos_model_id = 0;
PartsStockWindow stock;
Repair device;



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

        Combo(device, "##Models", device.model);
        Combo(device, "##Categories", device.category);

     
        if (device.model.current_id >= 0) {
            PartsStockWindow stock;
            stock.ResetOnModelChange(device.model, device.color, previos_model_id);
 
            Combo(device, "##Colors", device.color);
        }
        else {
            ImGui::Text("Choose model to show colors.");
        }
        
        ImGui::SeparatorText("Notes:");
        ImGui::InputTextWithHint("##Notes", "Notes visible for customer..", notes, IM_ARRAYSIZE(notes), ImGuiInputTextFlags_None);
        ImGui::InputTextWithHint("##Notes_hidden", "Notes hidden from customer..", notes_hidden, IM_ARRAYSIZE(notes_hidden), ImGuiInputTextFlags_None);

        ImGui::SeparatorText("Status:");
        Combo(device, "##States", device.state);

        ImGui::SeparatorText("Price:");
        ImGui::InputFloat("input float", &device.price, 0.1f, 1.0f, "%.2f"); 

        ImGui::TableNextColumn();

        ImGui::EndTable();
        if (ImGui::Button("Add repair")) {
            std::cout << device.model.current_id << std::endl;
            //SubmitRepair(this->errorMessage);
        }
     
        modalController.GetErrorState("Repair Input Feedback", this->errorMessage.c_str());

        if (ImGui::Button("test")) {
            for (int i = 0; i < device.model.data.size() ; i++)
            {
                std::cout << device.model.data[i] << std::endl;
            }
        }
    }
    ImGui::End();
}

void AddRepair::SubmitRepair(std::string& message) {
    CustomerPopulate customer;
    int submit = customer.Submit(repair_fields, repair_customer);
    SQLQuery sql;

    if (submit == -2) {
        modalController.RenderErrorModal("Repair Input Feedback");
        message = "Phone number can't be empty. It is used to identify customers.";
        return;
    }
    if (submit == 0) { // Here if customer does not exist we add him to database, and we are adding repair after.
        device.note = notes;
        device.note_hidden = notes_hidden;
        int cust_ID = sql.InsertCustomer(repair_customer);
        sql.AddRepair(device, cust_ID);
        for (InputField& field : repair_fields) {
            memset(field.buffer, 0, field.bufferSize); // Set all characters to null (clear the buffer)
        }
        return;
    }
    if (submit == -1) {
        modalController.RenderErrorModal("Repair Input Feedback");
        message = "There is a problem with inputting customers. Please contact support team.";
        return;

    }
    else { // Here we take this customer ID and add repair with this id.
        device.note = notes;
        device.note_hidden = notes_hidden;
        sql.AddRepair(device, submit);
        //modalController.RenderErrorModal("Repair Input Feedback");
        //message = "Customer with this phone number already exists. \nYou can edit his details. Right click on customer in table view and click a button.";
        return;
    }

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
void Combo(Repair& device, std::string label, PartData& attribute) {
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
        else if (label == "##States") {
            stock.GetStates(device.state);
        }
        attribute.current_id = 0;
        attribute.retreived = true;
    }
    ImGuiHelper helper;
    helper.ComboForDevice(label.c_str(), attribute);
}

