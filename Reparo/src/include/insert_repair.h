#pragma once

#include <time.h>
#include <windows.h>

#include "insert_customer.h"
#include "imguidatechooser.h"
#include "include/IObserver.h"

class InsertRepair : public InsertCustomer, public ISubject {
public:
    InsertRepair();
    InsertRepair(Repair& repair_);

public:
    virtual void Render() override;

protected:
    HintInputFieldsW_Popup model;
    HintInputFieldsW_Popup category;
    HintInputFieldsW_Popup color;
    Repair repair;
    const char* modal_message = "";
    double price = 0;
    HintInputField visible_note;
    HintInputField hidden_note;
    HintInputField sn_imei;

protected:
    // Sections to render
    void CustomerSection();
    void PhoneFieldSection() override;
    void DeviceSection();
    void CustomDeviceFeedback();
    void NotesSection();
    void PriceSection();
    void SerialNumSection();
    virtual void StateSection();

    // Buffers validators
    bool DeviceFieldsValidated();
    virtual bool RepairValidated();
    bool ChkBufferInDb(const char* label, const char* buffer);

    // Modals
    virtual void InitModal() override;
    virtual void RunModal(Repair& repair);

    // Misc
    virtual Repair InitRepair();
    void ResetOnEmptyMain() override;
    void InsertRepairButtonEnabler();
    virtual void InsertRepairButton();
    void InsertCustButton() override;
    void ResetFields() override;
    virtual void TestButton();

private:
  bool model_field_empty = false;
  bool custom_device = false;

// ISubject
public:
  void Attach(IObserver* observer) override;
  void Detach(IObserver* observer) override;

protected:
  void Notify() override;
  std::vector<IObserver*> list_observer;

private:  

protected:

};