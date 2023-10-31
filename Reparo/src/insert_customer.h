#pragma once
#include "imgui.h"
#include "structs.h"

class InsertCustomer {
public:
    InsertCustomer() {}

public:
    void Render();

private:
    Customer customer;
    char phone[128] = "";
    char name[128] = "";
    char surname[128] = "";
    char email[128] = "";
private:
    bool name_field;
    void CreateInputField(const char* label, const char* hint, char* buffer, bool& field);
};