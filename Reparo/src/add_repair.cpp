#include "add_repair.h"
#include <iostream> // delete later


char searchQuery[128] = "";

char repair_name[128] = "";
char repair_surname[128] = "";
char repair_email[128] = "";
char repair_phone[128] = "";
char notes[512] = "";
char notes_hidden[512] = "";

char input_model[64] = { "" };
bool is_input_model_enter_pressed;



int previos_model_id = 0;
PartsStockWindow stock;
Repair device;



std::vector<InputField> repair_fields = {
{"##PhoneNumber", "Phone Number..", repair_phone, IM_ARRAYSIZE(repair_phone), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank},
{"##Name", "Name..", repair_name, IM_ARRAYSIZE(repair_name), ImGuiInputTextFlags_None},
{"##Surname", "Surname..", repair_surname, IM_ARRAYSIZE(repair_surname), ImGuiInputTextFlags_None},
{"##Email", "Email..", repair_email, IM_ARRAYSIZE(repair_email), ImGuiInputTextFlags_None},
};

void AddRepair::AddRepairWindow() {

    ImGui::Begin("Add Repair");
    if (ImGui::BeginTable("Add repair table", 2, ImGuiTableFlags_Borders | ImGuiTableColumnFlags_IsHovered)) {

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::PushItemWidth(-1);

        ImGui::SeparatorText("CUSTOMER:");
        //CustomerInputWindow customerInput;
        customerInput.Render(CustomerInputFlags_NoSurnameField);

        //if (test) {
        //    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        //}
        //else {
        //    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        //}
        ImGui::SeparatorText("DEVICE:");
        //ImGui::PopStyleColor(1);

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
            if (customerInput.TestSubmitCall("Repair Input Feedback", CustomerSubmissionFlags_RepairAdd)) {
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
        //modalController.GetErrorState("Repair Input Feedback", this->errorMessage.c_str(), );
        if (ImGui::Button("test")) {
            sql.Prices();
        }
        //std::cout << row << std::endl;
        ImGui::TableNextColumn();
        ImGui::InputTextWithHint("##Search", "Search for customer...", searchQuery, IM_ARRAYSIZE(searchQuery), ImGuiInputTextFlags_None);
        SearchForCustomers();
        SearchForByPhone();

        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::EndTable();

    }
    ImGui::End();
}

void AddRepair::SubmitRepair() {
    //device.customer = customerInput.FieldsToCustomer();
    submit = sql.SearchForCustomerSQL(device.customer);
}

void AddRepair::ProcessSubmission() {
    //if (submit == PhoneNumberIsEmpty) {
    //    modalController.RenderErrorModal("Repair Input Feedback");
    //    error_message = "Phone number can't be empty. It is used to identify customers.";
    //    return;
    //}

    //if (submit == AddNewCustomer) { // Here if customer does not exist we add him to database, and we are adding repair after.
    //    GetIDs();
    //    int cust_ID = sql.InsertCustomer(device.customer);
    //    sql.AddRepair(device, cust_ID);
    //    for (InputField& field : repair_fields) {
    //        memset(field.buffer, 0, field.bufferSize); // Set all characters to null (clear the buffer)
    //    }
    //    return;
    //}

    //if (submit == WrongQuery) {
    //    modalController.RenderErrorModal("Repair Input Feedback");
    //    error_message = "There is a problem with inputting customers. Please contact support team.";
    //    return;

    //}
    //else { // Here we take this customer ID and add repair with this id.
    //    GetIDs();
    //    sql.AddRepair(device, submit);
    //    return;
    //}
}

void AddRepair::Models() {
    //Search search;
    if (search.SearchModel(pop_model, device.model.data)) {
        std::string strSearchQuery(pop_model.input);
        sql.MatchingModels(strSearchQuery, device.model.data, "models");
    }    
    const char* label = "##model";
    search.PopupModels(pop_model, device.model, label);
}

void AddRepair::Categories() {
    //Search search;
    if (search.SearchModel(pop_category, device.category.data)) {
        std::string strSearchQuery(pop_category.input);
        sql.MatchingModels(strSearchQuery, device.category.data, "categories");
    }
    const char* label = "##cat";
    search.PopupModels(pop_category, device.category, label);
}

void AddRepair::Colors() {
    //Search search;
    PartsStockWindow parts;
    parts.GetColorsForModel(device.color.data, device.model.data, device.model.name);
    const char* label = "##color";
    search.PopupModels(pop_color, device.color, label);
}

void AddRepair::SearchForByPhone() {
    //Search search;
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Looks like this customer already exists. \nDouble click on him to copy data over.");
    search.SearchField(searchQuery);
    search.ForAdd(customerInput.inputFields, SearchFlags_CopyToFields);
}

void AddRepair::SearchForCustomers() {
    //Search search;
    search.SearchField(customerInput.inputFields[0].buffer);
    search.ForAdd(customerInput.inputFields, SearchFlags_CopyToFields | SearchFlags_EditCustomer);
}

void Combo(Repair& device, std::string label, PartData& attribute) {
    if (!attribute.retreived) {
        if (label == "##States") {
            stock.GetStates(device.state);
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
