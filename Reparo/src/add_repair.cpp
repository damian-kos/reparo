#include "add_repair.h"
#include <iostream> // delete later


bool ntest = false;
bool test = true;
Repair device;


void AddRepair::AddRepairWindow() {

    ImGui::Begin("Add Repair");
    if (ImGui::BeginTable("Add repair table", 2, ImGuiTableFlags_Borders | ImGuiTableColumnFlags_IsHovered)) {

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::PushItemWidth(-1);

        //decorator.DecoratedSeparatorText("CUSTOMER:");
        //decorator.SetTestValue(test);
        customerInput.Render(CustomerInputFlags_NoSurnameField);
        if (ImGui::IsItemActive()) {
            ImGui::Text("????");
        }

        decorator.SetTestValue(test);
        decorator.DecoratedSeparatorText("DEVICE:");

        pop_model.is_input_enter_pressed = ImGui::InputTextWithHint("##Model_search", "Model..", pop_model.input, IM_ARRAYSIZE(pop_model.input), ImGuiInputTextFlags_EnterReturnsTrue);
        Models();
        pop_category.is_input_enter_pressed = ImGui::InputTextWithHint("##Category_search", "Type..", pop_category.input, IM_ARRAYSIZE(pop_category.input), ImGuiInputTextFlags_EnterReturnsTrue);
        Categories();

        pop_color.is_input_enter_pressed = ImGui::InputTextWithHint("##Color", "Color..", pop_color.input, IM_ARRAYSIZE(pop_color.input), ImGuiInputTextFlags_EnterReturnsTrue);
        Colors();

        ImGui::SeparatorText("Notes:");
        ImGui::InputTextWithHint("##Notes", "Notes visible for customer..", notes, IM_ARRAYSIZE(notes), ImGuiInputTextFlags_None);
        ImGui::InputTextWithHint("##Notes_hidden", "Notes hidden from customer..", notes_hidden, IM_ARRAYSIZE(notes_hidden), ImGuiInputTextFlags_None);

        ImGui::SeparatorText("Status:");
        Combo(device, "##States", device.state);

        ImGui::SeparatorText("Price:");
        ImGui::InputDouble("input float", &device.price, 0.1f, 1.0f, "%.2f");

        if (ImGui::Button("Add repair")) {
            SubmitRepair();
            if (customerInput.TestSubmitCall(CustomerSubmissionFlags_RepairAdd)) {
                GetIDs();
                if (customerInput.submit_customer_result == AddNewCustomer) {
                    int cust_ID = sql.InsertCustomer(device.customer);
                    sql.AddRepair(device, cust_ID);
                }
                else {
                    sql.AddRepair(device, customerInput.submit_customer_result);
                }
            }
        }

        if (ImGui::Button("test")) {
            sql.Prices();
        }
        //std::cout << row << std::endl;
        ImGui::TableNextColumn();
        ImGui::SeparatorText("GENERAL SEARCH:");
        ImGui::InputTextWithHint("##Search", "Search for customer...", searchQuery, IM_ARRAYSIZE(searchQuery), ImGuiInputTextFlags_None);
        SearchForCustomers();
        ImGui::SeparatorText("SIMILAR CUSTOMERS:");
        SearchForByPhone();
        ImGui::EndTable();
    }
    search.TestCust();
    ImGui::End();
}

void AddRepair::SubmitRepair() {
    device.customer = customerInput.FieldsToCustomer(CustomerInputFlags_NoSurnameField);
    customerInput.submit_customer_result = sql.SearchForCustomerSQL(device.customer);
}

void AddRepair::Models() {
    if (search.SearchModel(pop_model, device.model.data)) {
        std::string strSearchQuery(pop_model.input);
        sql.MatchingModels(strSearchQuery, device.model.data, "models");
    }    
    const char* label = "##model";
    search.PopupModels(pop_model, device.model, label);
}

void AddRepair::Categories() {
    if (search.SearchModel(pop_category, device.category.data)) {
        std::string strSearchQuery(pop_category.input);
        sql.MatchingModels(strSearchQuery, device.category.data, "categories");
    }
    const char* label = "##cat";
    search.PopupModels(pop_category, device.category, label);
}

void AddRepair::Colors() {
    PartsStockWindow parts;
    parts.GetColorsForModel(device.color.data, device.model.data, device.model.name);
    const char* label = "##color";
    search.PopupModels(pop_color, device.color, label);
}

void AddRepair::SearchForByPhone() {
    search.SearchField(customerInput.inputFields[0].buffer);
    search.ForAdd(customerInput.inputFields, 2, SearchFlags_CopyToFields | SearchFlags_EditCustomer);
}

void AddRepair::SearchForCustomers() {
    search.SearchField(searchQuery);
    search.ForAdd(customerInput.inputFields, 1, SearchFlags_CopyToFields);
}

void AddRepair::Combo(Repair& device, std::string label, PartData& attribute) {
    PartsStockWindow parts;
    if (!attribute.retreived) {
        if (label == "##States") {
            parts.GetStates(device.state);
        }
        attribute.current_id = 0;
        attribute.name = attribute.data[attribute.current_id];
        attribute.retreived = true;
    }
    ImGuiHelper helper;
    helper.ComboForDevice(label.c_str(), attribute);
}

void AddRepair::GetIDs() {
    device.note = notes;
    device.note_hidden = notes_hidden;
    std::string query = "SELECT model_id FROM models WHERE model = ? ";
    device.model.IDinDB = sql.GetIdForValue(query, device.model.name.c_str());
    query = "SELECT category_id FROM categories WHERE category = ? ";
    device.category.IDinDB = sql.GetIdForValue(query, device.category.name.c_str());
    query = "SELECT color_id FROM colors WHERE color = ? ";
    device.color.IDinDB = sql.GetIdForValue(query, device.color.name.c_str());
    query = "SELECT repair_state_id FROM repair_states WHERE repair_state = ? ";
    device.state.IDinDB = sql.GetIdForValue(query, device.state.name.c_str());
}
