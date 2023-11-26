#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include "structs.h"
#include "imgui_viewer.h"
#include "database.h"
#include "modals.h"



namespace ImGui {
    //Database db;
    //ModalController modals;

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

    void InputTextWithHintExt(const char* label, const char* hint, HintInputField& field, std::function<bool()> validation_function, bool* feedback) {
        ImGui::InputTextWithHint(label, hint, field.buffer, 128, field.imgui_flags);

        if (ImGui::IsItemEdited() && validation_function) {
            std::cout << "Item is edited " << std::endl;
            field.validated = validation_function();
        }

        if (ImGui::IsItemDeactivated() && feedback) {
            *feedback = true;
        }
    }

    void InputTextWithPopup(const char* label, const char* hint, HintInputFieldsW_Popup& field, std::function<bool()> validation_function, bool* selection, HintInputFieldsW_Popup* rel_field, bool* feedback) {
        if (field.input.is_on) {
            ImGui::InputTextWithHintExt(label, hint, field.input, validation_function, feedback);
            if (rel_field) {
                if (rel_field->input.validated && !field.attribute.retreived) {
                    int id = Database::GetIDForValueS(label, rel_field->input.buffer);
                    Database::ManageSearchState(label, field.attribute, id, field.input.buffer);
                    field.attribute.retreived = true;
                }
                if (!rel_field->input.validated && field.attribute.retreived) {
                    field.attribute.retreived = false;
                }
            }
            else if (ImGui::IsItemEdited() || ImGui::IsItemActivated()) {
                Database::ManageSearchState(label, field.attribute, field.input.buffer);
            }
            ModalController::PopupOnInputField(field, selection, label);
        }
    }

}



