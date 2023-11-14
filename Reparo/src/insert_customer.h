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
    HintInputField phone{ ImGuiInputTextFlags_CharsDecimal };
    HintInputField name;
    HintInputField surname;
    HintInputField email;

private:
    std::string validation_feedback = "";
    ConfirmResult result = ConfirmResult::ConfirmIdle;
protected:
    Database db;
    ImGuiDecorator imgui_decorator;
    ModalController modals;
protected:
    void CreateInputField(const char* label, const char* hint, HintInputField& field, std::function<bool()> validation_function);
    virtual void SubmitButton();
    bool IsEmailValid(std::string email);
    bool SimpleValidation(const char* buffer, int length);
    bool FieldsValidated();
    int SetValidaitonErr();
    void SetValidationMsg();
    Customer InitCustomer();
    void InitModal();
    void RunModal(Customer& customer);

    void ResetFields();


};