#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <map>

#include "imgui.h"
#include "imgui_decorator.h"
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
    Database db;
    ImGuiDecorator imgui_decorator;
    ModalController modals;

private:
    std::string validation_feedback = "";
    Customer* temp_customer = nullptr;

    bool selected;

protected:
    void CreateInputField(const char* label, const char* hint, HintInputField& field, std::function<bool()> validation_function);
    void CustomerAlreadyExists();
    void CustomerSelectedOnPopup();
    void CreateInputField(const char* label, const char* hint, HintInputFieldsW_Popup& field, std::function<bool()> validation_function);
    virtual void PopupFields(const char* label, HintInputFieldsW_Popup& field);
    void FieldsSection();
    virtual void SubmitButton();
    bool IsEmailValid(std::string email);
    bool SimpleValidation(const char* buffer, int length);
    bool FieldsValidated();
    int SetValidaitonErr();
    void UpdateValidationMsg();
    Customer InitCustomer();
    virtual void InitModal();
    void RunModal(Customer& customer);
    virtual void ResetFields();
    void CopyToBuffer(char* buffer, std::string, bool& field_validation, std::function<bool()> validation_function);
};