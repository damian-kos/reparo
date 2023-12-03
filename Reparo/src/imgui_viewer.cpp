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


  void ViewCustomer(const Customer& customer, std::shared_ptr<Customer> t_customer) {
    ImGui::TableNextColumn();
    ImGui::BeginGroup();
    ImGui::Text("Phone:    "); ImGui::SameLine(); ImGui::Text(customer.phone.c_str());
    ImGui::Text("Name:     "); ImGui::SameLine(); ImGui::Text(customer.name.c_str());
    ImGui::Text("Surname:  "); ImGui::SameLine(); ImGui::Text(customer.surname.c_str());
    ImGui::Text("Email:    "); ImGui::SameLine(); ImGui::Text(customer.email.c_str());
    ImGui::EndGroup();
    if (t_customer) {
      ImGui::TableNextColumn();
      ImGui::SameLine();
      ImGui::BeginGroup();
      std::string tab = "    >>>>    ";
      ImGui::Text((customer.phone != t_customer->phone) ? (tab + (t_customer->phone)).c_str() : "");
      ImGui::Text((customer.name != t_customer->name) ? (tab + (t_customer->name)).c_str() : "");
      ImGui::Text((customer.surname != t_customer->surname ) ? (tab + (t_customer->surname)).c_str() : "");
      ImGui::Text((customer.email != t_customer->email) ? (tab + (t_customer->email)).c_str() : "");
      ImGui::EndGroup();
    }
  }

  void ViewRepair(const Repair& repair, std::shared_ptr<Repair> t_repair, 
                  std::shared_ptr<Customer> t_customer) {
    
    ImGui::BeginTable("table1", 2);
    ImGui::ViewCustomer(repair.customer, t_customer);
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::BeginGroup();
    ImGui::Text("Model:    "); ImGui::SameLine(); ImGui::Text(repair.device.name.c_str());
    ImGui::Text("Category: "); ImGui::SameLine(); ImGui::Text(repair.category.c_str());
    ImGui::Text("Color:    "); ImGui::SameLine(); ImGui::Text(repair.device.color.c_str());
    ImGui::Text("State:    "); ImGui::SameLine(); ImGui::Text(repair.state.c_str());
    ImGui::Text("Price:    "); ImGui::SameLine(); ImGui::Text("%.2f", repair.price);
    ImGui::EndGroup();
    if (t_repair) {
    ImGui::TableNextColumn();
    ImGui::SameLine();
    ImGui::BeginGroup();
    std::string tab = "    >>>>    ";
    ImGui::Text((repair.device.name != t_repair->device.name) ?
                (tab + (t_repair->device.name)).c_str() : "");
    ImGui::Text((repair.category != t_repair->category) ?
                (tab + (t_repair->category)).c_str() : "");
    ImGui::Text((repair.device.color != t_repair->device.color) ?
                (tab + (t_repair->device.color)).c_str() : "");
    ImGui::Text((repair.state != t_repair->state) ?
      tab.append(t_repair->state).c_str() : "");
    double tolerance = 1e-3;
    if (!(std::abs(repair.price - t_repair->price) < tolerance)) {
      ImGui::Text("    >>>>    %.2f", t_repair->price);
    }
    ImGui::EndGroup();
    }
    ImGui::EndTable();
  }

  void SeparatorDecorator(const char* label, bool decorated) {
    if (decorated) {
      ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    }
    else {
      ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    ImGui::PushItemWidth(100);
    ImGui::SeparatorText(label);

    ImGui::PopStyleColor(1);
    ImGui::PopItemWidth();

  }

  void SeparatorTextAlignR(const char* label) {
    ImGuiStyle* style = &ImGui::GetStyle();
    style->SeparatorTextAlign = ImVec2(1.0f, 0.5f);
    ImGui::SeparatorText(label);
    style->SeparatorTextAlign = ImVec2(0.0f, 0.5f);
  }

  void InputTextWithHintExt(const char* label, const char* hint, HintInputField & field, std::function<bool()> validation_function, bool* feedback) {
    ImGui::InputTextWithHint(label, hint, field.buffer, 128, field.imgui_flags);

    if (ImGui::IsItemEdited() && validation_function) {
      std::cout << "Item is edited " << std::endl;
      field.validated = validation_function();
    }

    if (ImGui::IsItemDeactivated() && feedback) {
      *feedback = true;
    }
  }

  bool ButtonScaled(const char* label, float font_size, ImVec2 button_size)  {
    float old_size = ImGui::GetFont()->Scale;
    ImGui::GetFont()->Scale *= font_size;
    ImGui::PushFont(ImGui::GetFont());
    bool value = false;
    if (ImGui::Button(label, button_size))
      value = true;
    ImGui::GetFont()->Scale = old_size;
    ImGui::PopFont();
    return value;

  }



  void InputTextWithPopup(const char* label, const char* hint, HintInputFieldsW_Popup & field, std::function<bool()> validation_function, bool* selection, HintInputFieldsW_Popup * rel_field, bool* feedback) {
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




