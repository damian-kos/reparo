#pragma once

#include <time.h>

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
    void ResetOnEmptyMain() override;
    void CustomerSection();
    void PhoneFieldSection() override;
    void DeviceSection();
    void NotesSection();
    void PriceSection();
    void SerialNumSection();
    virtual void StateSection();
    void InsertRepairButtonEnabler();
    virtual void InsertRepairButton();
    bool DeviceFieldsValidated();
    virtual bool RepairValidated();
    bool BufferQueryOnDatabase(const char* label, const char* buffer);
    virtual Repair InitRepair();
    virtual void InitModal() override;
    virtual void RunModal(Repair& repair);
    void InsertCustButton() override;
    void ResetFields() override;
    virtual void TestButton();

private:
  bool model_field_empty = false;

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