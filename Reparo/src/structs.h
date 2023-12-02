#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "imgui.h"

#include "enums.h"

struct Customer {
public:
    Customer()
        : phone(""), name(""), surname(""), email("") {}

    Customer(const std::string& phone, const std::string& name,
        const std::string& surname, const std::string& email)
        : phone(phone), name(name), surname(surname), email(email) {}

    ~Customer() { /*std::cout << "Customer destroyed" << std::endl;*/ }

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
    bool is_input_enter_pressed;
    bool is_input_active;
    bool is_input_activated;
    bool is_widnow_active;
    Attribute attribute;
};

struct Device {
    Device()
        : name(""), color("") {
       /* std::cout << "Device created empty" << std::endl;*/
    }
    Device(std::string name, std::string color)
        : name(name), color(color) {
        /*std::cout << "Device created with values " << std::endl;*/
    }
    ~Device() { /*std::cout << "Device destroyed " << std::endl;*/ }
    std::string name;
    std::string color;
};

struct Repair {
    Repair()
            : customer(), device(), category(""), price(0), visible_note(""), hidden_note(""), state(""), date("") {}

    Repair(const Customer& cust, const Device& dev, std::string cat, double price, std::string visible_note, std::string hidden_note, std::string state)
        : customer(cust), device(dev), category(cat), price(price), visible_note(visible_note), hidden_note(hidden_note), state(state){}

    Repair(const Customer& cust, const Device& dev, std::string cat, double price, std::string visible_note, std::string hidden_note, std::string state, std::string date)
        : customer(cust), device(dev), category(cat), price(price), visible_note(visible_note), hidden_note(hidden_note), state(state), date(date) {}
   
    ~Repair() { /*std::cout << "Repair destroyed " << std::endl;*/ }

    Customer customer;
    Device device;
    std::string category;
    double price;
    std::string visible_note;
    std::string hidden_note;
    std::string state;
    std::string date;
};