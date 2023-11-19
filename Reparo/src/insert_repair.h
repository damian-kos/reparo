#pragma once
#include "insert_customer.h"

class InsertRepair : public InsertCustomer {
public:
    InsertRepair();
    InsertRepair(Repair& repair_);
public:
    void Render() override;
    void DeviceSection();
    void NotesSection();
    void PriceSection();
    virtual void StateSection();
    void InsertRepairButtonEnabler();
    virtual void InsertRepairButton();
    //void CreateInputField(const char* label, const char* hint, HintInputFieldsW_Popup& field, std::function<bool()> validation_function);
    void CreateInputField(const char* label, const char* hint, HintInputField& field);
    bool DeviceFieldsValidated();
    bool RepairValidated();
    
    void PopupFields(const char* label, HintInputFieldsW_Popup& field, HintInputFieldsW_Popup& rel_field);
    bool BufferQueryOnDatabase(const char* label, const char* buffer);
    Repair InitRepair();
    virtual void InitModal() override;
    virtual void RunModal(Repair& repair);
    void SubmitButton() override;
    void ResetFields() override;
    virtual void TestButton();
    Repair repair;
protected:
    const char* modal_message;
    double price = 0;
    bool device_validated = false;
    HintInputFieldsW_Popup model;
    HintInputFieldsW_Popup category;
    HintInputFieldsW_Popup color;
    HintInputField visible_note;
    HintInputField hidden_note;
};