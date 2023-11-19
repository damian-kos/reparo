#pragma once
#include "insert_customer.h"

class InsertRepair : public InsertCustomer {
public:
    InsertRepair();
    InsertRepair(Repair& repair);
public:
    void Render() override;
    void DeviceSection();
    void NotesSection();
    void PriceSection();
    virtual void StateSection();
    void InsertRepairButton();
    //void CreateInputField(const char* label, const char* hint, HintInputFieldsW_Popup& field, std::function<bool()> validation_function);
    void CreateInputField(const char* label, const char* hint, HintInputField& field);
    bool DeviceFieldsValidated();
    bool RepairValidated();
    
    void PopupFields(const char* label, HintInputFieldsW_Popup& field, HintInputFieldsW_Popup& rel_field);
    bool BufferQueryOnDatabase(const char* label, const char* buffer);
    Repair InitRepair();
    void InitModal() override;
    void RunModal(Repair& customer);
    void SubmitButton() override;
    void ResetFields() override;
    void TestButton();
    Repair repair;
private:
    double price = 0;
    bool device_validated = false;
    HintInputFieldsW_Popup model;
    HintInputFieldsW_Popup category;
    HintInputFieldsW_Popup color;
    HintInputField visible_note;
    HintInputField hidden_note;

};