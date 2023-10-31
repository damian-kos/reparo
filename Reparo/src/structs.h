#pragma once
#include <string>

struct Customer {
public:
    Customer()
        : phone(""), name(""), surname(""), email("") {}

    Customer(const std::string& phone, const std::string& name,
        const std::string& surname, const std::string& email)
        : phone(phone), name(name), surname(surname), email(email) {}

    ~Customer() {}

    std::string phone;
    std::string name;
    std::string surname;
    std::string email;
};