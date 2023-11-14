#pragma once
#include "insert_customer.h"

class InsertRepair : public InsertCustomer {
public:
    void Render() override;
    void DeviceSection();
    void SubmitButton() override;
    void CreateInputField(const char* label, const char* hint, HintInputFieldsW_Popup& field, std::function<bool()> validation_function);
    void CreateInputField(const char* label, const char* hint, HintInputField& field);
    bool DeviceFieldsValidated();
    void PopupFields(const char* label, HintInputFieldsW_Popup& field);
    void PopupFields(const char* label, HintInputFieldsW_Popup& field, HintInputFieldsW_Popup& rel_field);
    bool BufferQueryOnDatabase(const char* label, const char* buffer);
private:
    bool device_validated;
    HintInputFieldsW_Popup model;
    HintInputFieldsW_Popup category;
    HintInputFieldsW_Popup color;
    HintInputField visible_note;
    HintInputField hidden_note;

};