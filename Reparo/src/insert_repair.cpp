#include "insert_repair.h"

void InsertRepair::Render() {
    InsertCustomer::Render();
    DeviceSection();
    NotesSection();
    PriceSection();
}

void InsertRepair::DeviceSection() {
    imgui_decorator.SetTestValue(DeviceFieldsValidated());
    imgui_decorator.DecorateSeparatorText("DEVICE: ");
    CreateInputField("##Model", "Model of device...", model, [&]() { return BufferQueryOnDatabase("##Model", model.input.buffer); });
    PopupFields("##Model", model);
    modals.PopupOnInputField(model, "model");
    CreateInputField("##Category", "Category...", category, [&]() { return BufferQueryOnDatabase("##Category", category.input.buffer); });
    PopupFields("##Category", category);
    modals.PopupOnInputField(category, "category");
    CreateInputField("##Color", "Color...", color, [&]() { return BufferQueryOnDatabase("##Color", color.input.buffer); });
    PopupFields("##Color", color, model);
    modals.PopupOnInputField(color, "color");

    //Debugging
    ImGui::Text(model.input.validated ? "true" : "false");
    ImGui::SameLine(); ImGui::Text(category.input.validated ? "true" : "false");
    ImGui::SameLine(); ImGui::Text(color.input.validated ? "true" : "false");
    //
}

void InsertRepair::NotesSection()
{
    imgui_decorator.SetTestValue(true);
    imgui_decorator.DecorateSeparatorText("DEVICE: ");
    CreateInputField("##VisibleNote", "Note for customer...", visible_note);
    CreateInputField("##VisibleNote", "Note for shop...", hidden_note);
}

void InsertRepair::PriceSection()
{
    imgui_decorator.SetTestValue((price > 0) ? true : false);
    imgui_decorator.DecorateSeparatorText("PRICE: ");
    ImGui::InputDouble("input float", &price, 0.1f, 1.0f, "%.2f");
}

void InsertRepair::CreateInputField(const char* label, const char* hint, HintInputFieldsW_Popup& field, std::function<bool()> validation_function) {
    if (field.input.is_on) {
        ImGui::InputTextWithHint(label, hint, field.input.buffer, 128, field.input.imgui_flags);
        if (ImGui::IsItemEdited()) {
            field.input.validated = validation_function();
        }
        if (ImGui::IsItemDeactivated()) {

        }
    }
}

void InsertRepair::CreateInputField(const char* label, const char* hint, HintInputField& field) {
    if (field.is_on) 
        ImGui::InputTextWithHint(label, hint, field.buffer, 128, field.imgui_flags);
}

bool InsertRepair::DeviceFieldsValidated() {
    return (model.input.validated && category.input.validated && color.input.validated);
}

void InsertRepair::PopupFields(const char* label, HintInputFieldsW_Popup& field) {
    if (ImGui::IsItemEdited() || ImGui::IsItemActivated()) {
        db.ManageSearchState(label, field.attribute, field.input.buffer);
    }
}

void InsertRepair::PopupFields(const char* label, HintInputFieldsW_Popup& field, HintInputFieldsW_Popup& rel_field) {
    if (rel_field.input.validated) {
        int id = db.GetIDForValue(label, rel_field.input.buffer);
        db.ManageSearchState(label, field.attribute, field.input.buffer, id);
    }
}

bool InsertRepair::BufferQueryOnDatabase(const char* label, const char* buffer) {
    return db.GetBoolForValue(label, buffer);
}

void InsertRepair::SubmitButton() {
    If
}