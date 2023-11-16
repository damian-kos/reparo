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
    ~InsertCustomer();
public:
    virtual void Render();


private:
    HintInputFieldsW_Popup phone{ ImGuiInputTextFlags_CharsDecimal };
    HintInputField name;
    HintInputField surname;
    HintInputField email;

private:
    std::string validation_feedback = "";
protected:
    ConfirmResult result = ConfirmResult::CONIFRM_IDLE;
    Database db;
    ImGuiDecorator imgui_decorator;
    ModalController modals;
protected:
    void CreateInputField(const char* label, const char* hint, HintInputField& field, std::function<bool()> validation_function);
    void CreateInputField(const char* label, const char* hint, HintInputFieldsW_Popup& field, std::function<bool()> validation_function);
    virtual void PopupFields(const char* label, HintInputFieldsW_Popup& field);
    void FieldsSection();
    virtual void SubmitButton();
    bool IsEmailValid(std::string email);
    bool SimpleValidation(const char* buffer, int length);
    bool FieldsValidated();
    int SetValidaitonErr();
    void SetValidationMsg();
    Customer InitCustomer();
    virtual void InitModal();
    void RunModal(Customer& customer);

    virtual void ResetFields();


};