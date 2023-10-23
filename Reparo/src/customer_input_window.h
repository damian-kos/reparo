#pragma once

#include "imgui.h"
#include "search.h"
#include "error_handler.h"
#include "structs.h"
#include "sql_queries.h"
#include "customer_fields.h"

class CustomerInputWindow {
public:
    
    CustomerInputWindow(std::string title) :  decorator(false){
        error_title = title;
    }
    CustomerInputWindow(std::string title, CustomerInputFlags input_flags) : decorator(false) {
        error_title = title;
        reparo_flags = input_flags;
    }

public:
    void Render(CustomerInputFlags = 0);
    void InputValidation(InputField& input);
    void CreateInputFields(CustomerInputFlags = 0);
    Customer FieldsToCustomer(CustomerInputFlags = 0);
    void Submit();
    bool TestSubmitCall(CustomerSubmissionFlags reparo_flags = 0);
    std::vector<InputField> inputFields = {
    {"##PhoneNumber", "Phone Number..", phoneNumber, IM_ARRAYSIZE(phoneNumber), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank},
    {"##Name", "Name..", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_None},
    {"##Surname", "Surname..", surname, IM_ARRAYSIZE(surname), ImGuiInputTextFlags_None},
    {"##Email", "Email..", email, IM_ARRAYSIZE(email), ImGuiInputTextFlags_None},
    };
    Customer customer;
    int submit_customer_result = NotSubmitted;
    ModalController modalController;

private:
    SQLQuery sql;
    ImGuiDecorator decorator;

private:
    bool test_bool;
    void PassSearchQuery();
    char name[128] = "";
    char surname[128] = "";
    char email[128] = "";
    char phoneNumber[128] = "";
    int phoneNumberIndex = -1; // Initialize to an invalid index
    bool inputError = false;
    std::string error_message = "";
    std::string error_title = "";
    CustomerInputFlags reparo_flags;

};


