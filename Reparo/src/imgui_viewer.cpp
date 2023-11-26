#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include "structs.h"
#include "imgui_viewer.h"


namespace ImGui {

//struct Customer {
//public:
//    Customer()
//        : phone(""), name(""), surname(""), email("") {}

//    Customer(const std::string& phone, const std::string& name,
//        const std::string& surname, const std::string& email)
//        : phone(phone), name(name), surname(surname), email(email) {}

//    ~Customer() { /*std::cout << "Customer destroyed" << std::endl;*/ }

//    std::string phone;
//    std::string name;
//    std::string surname;
//    std::string email;
//};


    void ViewCustomer(const Customer& customer) {
        ImGui::Text("Phone:    "); ImGui::SameLine(); ImGui::Text(customer.phone.c_str());
        ImGui::Text("Name:     "); ImGui::SameLine(); ImGui::Text(customer.name.c_str());
        ImGui::Text("Surname:  "); ImGui::SameLine(); ImGui::Text(customer.surname.c_str());
        ImGui::Text("Email:    "); ImGui::SameLine(); ImGui::Text(customer.email.c_str());
    }

    void ViewRepair(const Repair& repair) {
        ImGui::ViewCustomer(repair.customer);
        ImGui::Text("Model:    "); ImGui::SameLine(); ImGui::Text(repair.device.name.c_str());
        ImGui::Text("Category: "); ImGui::SameLine(); ImGui::Text(repair.category.c_str());
        ImGui::Text("Color:    "); ImGui::SameLine(); ImGui::Text(repair.device.color.c_str());
        ImGui::Text("Price:    "); ImGui::SameLine(); ImGui::Text("%.2f", repair.price);
    }

    void SeparatorDecorator(const char* label, bool decorated) {
        if (decorated) {
            ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        }
        ImGui::SeparatorText(label);

        ImGui::PopStyleColor(1);
    }
}

