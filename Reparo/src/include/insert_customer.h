#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <map>

#include "imgui.h"
#include "database.h"
#include "modals.h"

class InsertCustomer {
public:
    InsertCustomer();
    InsertCustomer(Customer& cust);

    ~InsertCustomer();

public:
    virtual void Render();


protected:
    HintInputFieldsW_Popup phone{ ImGuiInputTextFlags_CharsDecimal };
    HintInputField name;
    HintInputField surname;
    HintInputField email;
    Customer customer;
    ConfirmResult result = ConfirmResult::CONIFRM_IDLE;
    bool selected = false;
    bool feedback = false; // Probably can be replaced with Observer 
    bool phone_field_empty = false; // Phone field buffer should be utilized instead of this flag

    Customer* temp_customer = nullptr;

private:
    std::string validation_feedback = "";

protected:
    virtual void ResetOnEmptyMain();
    void CustomerSelectedOnPopup();
    void FieldsSection();
    virtual void PhoneFieldSection();
    virtual void InsertCustButton();
    bool IsEmailValid(std::string email);
    bool LenValidation(const char* buffer, int length);
    bool FieldsValidated();
    int SetValidaitonErr();
    void UpdateValidationMsg();
    Customer InitCustomer();
    virtual void InitModal();
    void RunModal(Customer& customer);
    virtual void ResetFields();
    void CopyToBuffer(char* buffer, std::string, bool& field_validation, 
                      std::function<bool()> validation_function);
};