#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "imgui.h"

struct Customer {
public:
    Customer()
        : phone(""), name(""), surname(""), email("") {}

    Customer(const std::string& phone, const std::string& name,
        const std::string& surname, const std::string& email)
        : phone(phone), name(name), surname(surname), email(email) {}

    ~Customer() { std::cout << "Customer destroyed" << std::endl; }

    std::string phone;
    std::string name;
    std::string surname;
    std::string email;
};

struct HintInputField {
    HintInputField() {}
    HintInputField(ImGuiInputTextFlags flags) : imgui_flags(flags) {}
    char buffer[128] = "";
    bool is_on = true;
    ImGuiInputTextFlags imgui_flags = ImGuiInputTextFlags_None;
    bool validated = false;
};

struct Attribute {
    std::vector<std::string> data;
    std::string name = "";
    bool retreived = false;
};

struct HintInputFieldsW_Popup  {
    HintInputField input { ImGuiInputTextFlags_EnterReturnsTrue };
    size_t prev_len;
    bool is_input_enter_pressed;
    bool is_input_active;
    bool is_input_activated;
    Attribute attribute;
};

