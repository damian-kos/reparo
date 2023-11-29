#pragma once

#include <time.h>

#include "insert_customer.h"
#include "imguidatechooser.h"

class InsertRepair : public InsertCustomer {
public:
    InsertRepair();
    InsertRepair(Repair& repair_);

public:
    void Render() override;

protected:
    HintInputFieldsW_Popup model;
    HintInputFieldsW_Popup category;
    HintInputFieldsW_Popup color;
    Repair repair;
    const char* modal_message = "";
    double price = 0;
    bool device_validated = false;
    HintInputField visible_note;
    HintInputField hidden_note;
    tm date = {};
    std::string str_date = "";

protected:
    void CustomerSection();
    void PhoneFieldSection() override;
    void DeviceSection();
    void NotesSection();
    void PriceSection();
    virtual void StateSection();
    void InsertRepairButtonEnabler();
    virtual void InsertRepairButton();
    bool DeviceFieldsValidated();
    bool RepairValidated();
    bool BufferQueryOnDatabase(const char* label, const char* buffer);
    virtual Repair InitRepair();
    virtual void InitModal() override;
    virtual void RunModal(Repair& repair);
    void SubmitButton() override;
    void ResetFields() override;
    virtual void TestButton();
};