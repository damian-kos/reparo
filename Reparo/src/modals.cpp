#include <sstream>
#include "modals.h"


ModalController::ModalController() {
  std::cout << "ModalControler created" << std::endl;
}

ModalController::~ModalController() {
  std::cout << "ModalController destroyed" << std::endl;
}

void ModalController::RenderModal(const char* modal_title) {
  std::cout << "RENDER MODAL: " << modal_title << std::endl;
  ImGui::OpenPopup(modal_title);
}

void ModalController::CenterAlign() {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void ModalController::ModalConfirm(const char* modal_title, Customer& customer, ConfirmResult& result) {
  CenterAlign();
  if (ImGui::BeginPopupModal(modal_title, NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    //ImGui::Text("Do you want to insert customer with this details?");
    ImGui::Separator();

    ImGui::ViewCustomer(customer, nullptr);
    if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup();  result = ConfirmResult::CONIFRM_SUBMIT; }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); result  = ConfirmResult::CONFIRM_CANCEL; }
    ImGui::EndPopup();
  }
}

void ModalController::ModalConfirm(const char* modal_title, Repair& repair, ConfirmResult& result) {
  CenterAlign();

  if (ImGui::BeginPopupModal(modal_title, NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    //ImGui::Text("Do you want to insert repair with this details?");
    ImGui::Separator();
    ImGui::ViewRepair(repair, nullptr, nullptr);

    if (ImGui::Button("OK", ImVec2(120, 0))) 
      { ImGui::CloseCurrentPopup();  result = ConfirmResult::CONIFRM_SUBMIT; }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) 
      { ImGui::CloseCurrentPopup(); result = ConfirmResult::CONFIRM_CANCEL; }
    ImGui::EndPopup();
  }
}

void ModalController::ModalConfirm(const char* modal_title, ConfirmResult& result, ID3D11ShaderResourceView* ticket) {
  CenterAlign();

  
  if (ImGui::BeginPopupModal(modal_title, NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Image((void*)ticket, ImVec2(210*3, 297*3));
    ImGui::Text("Do you want to print repair ticket?");
    ImGui::Separator();

    if (ImGui::Button("OK", ImVec2(120, 0)))
    {
      ImGui::CloseCurrentPopup();  result = ConfirmResult::CONIFRM_SUBMIT;
    }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
      ImGui::CloseCurrentPopup(); result = ConfirmResult::CONFIRM_CANCEL;
    }
    ImGui::EndPopup();
  }
}

void ModalController::ModalConfirm(const char* modal_title, Repair& repair, 
                                    std::shared_ptr<Repair> temp_repair, 
                                    std::shared_ptr<Customer> temp_customer, 
                                    ConfirmResult& result) {
  CenterAlign();
  if (ImGui::BeginPopupModal(modal_title, NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Do you want to update repair with this details?");
    ImGui::Separator();
    ImGui::ViewRepair(repair, temp_repair, temp_customer);

    if (ImGui::Button("OK", ImVec2(120, 0)))
    {
      ImGui::CloseCurrentPopup();  result = ConfirmResult::CONIFRM_SUBMIT;
    }
    ImGui::SetItemDefaultFocus();
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0)))
    {
      ImGui::CloseCurrentPopup(); result = ConfirmResult::CONFIRM_CANCEL;
    }
    ImGui::EndPopup();
  }
}

std::string SeperateData(const char* data, const char* label) {
    std::string data_string(data);
    if (label == "##Phone" || label=="##PartialPhone") {
        std::istringstream iss(data_string);
        std::string changed_data;
        iss >> changed_data;
        return changed_data;
    }
    else {
        return data_string;
    }
}

void ModalController::PopupOnInputField(HintInputFieldsW_Popup& field, 
                                        bool* selected, const char* label) {
  std::unordered_map<const char*, bool> prefix_text_on_popup = {
  {"##Phone", true},
  {"##PartialPhone", false},
  {nullptr, false},
  };

  field.is_input_active = ImGui::IsItemActive();
  field.is_input_activated = ImGui::IsItemActivated();

  if (field.is_input_activated) {
      ImGui::OpenPopup(label);
      std::cout << &field << label << std::endl;
  }

  ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
  if (ImGui::BeginPopup(label, ImGuiWindowFlags_NoTitleBar 
                        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize 
                        | ImGuiWindowFlags_ChildWindow))
  {
    if (!field.attribute.data.empty() && prefix_text_on_popup[label])
      ImGui::Text("Customer already exists");
    for (int i = 0; i < field.attribute.data.size(); i++)
    {
      if (ImGui::Selectable(field.attribute.data[i].c_str()))
      {
        strcpy(field.input.buffer, SeperateData(field.attribute.data[i].c_str(),
               label).c_str());
        field.input.valid = true;
        field.attribute.name = field.attribute.data[i];
        if (selected) {
            printf("PopupOnInputField run\n");
            *selected = true;
        }
      }
    }
    if (!field.is_input_active && !ImGui::IsWindowFocused()) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}
