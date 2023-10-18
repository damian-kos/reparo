#pragma once

#include "imgui.h"
#include "search.h"
#include "error_handler.h"
#include "structs.h"
#include "sql_queries.h"
#include "customer_fields.h"

enum CustomerInputFlags_ {
    CustomerInputFlags_None = 0,
    CustomerInputFlags_SubmitButton = 1 << 0,
    CustomerInputFlags_NameField = 1 << 2,
    CustomerInputFlags_NoSurnameField = 1 << 3,
    CustomerInputFlags_EmailField = 1 << 4,
    CustomerInputFlags_SearchResultsOnPhoneNo = 1 << 5,
};


class CustomerInputWindow {

public:
    void Render(CustomerInputFlags = 0);
    void CreateInputFields();
    Customer FieldsToCustomer();
    void Submit();
    void PassSearchQuery();

    std::vector<InputField> inputFields = {
    {"##PhoneNumber", "Phone Number..", phoneNumber, IM_ARRAYSIZE(phoneNumber), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank},
    {"##Name", "Name..", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_None},
    {"##Surname", "Surname..", surname, IM_ARRAYSIZE(surname), ImGuiInputTextFlags_None},
    {"##Email", "Email..", email, IM_ARRAYSIZE(email), ImGuiInputTextFlags_None},
    };
    int flags;
    ModalController modalController;
    Customer customer;

private:
    char name[128] = "";
    char surname[128] = "";
    char email[128] = "";
    char phoneNumber[128] = "";
    int phoneNumberIndex = -1; // Initialize to an invalid index
    bool inputError = false;
    std::string errorMessage = "";

};
