#pragma once
#include <unordered_map>
#include <memory>

#include "imgui.h"
#include "structs.h"
#include "imgui_viewer.h"
#include "enums.h"
//#include "repair_ticket.h"
//#include "updates.h"
class Logo;

class ModalController {
public:
  ModalController();
  ~ModalController();
public:
    
    static void RenderModal(const char* modal_title);
    static void ModalConfirm(const char* modal_title, Customer& customer, 
                              ConfirmResult& result);
    static void ModalConfirm(const char* modal_title, Repair& repair, 
                              ConfirmResult& result);
    static void ModalConfirm(const char* modal_title, ConfirmResult& result, ID3D11ShaderResourceView* ticket);
    static void ModalConfirm(const char* modal_title, Repair& repair, 
                              std::shared_ptr<Repair> temp_repair, 
                              std::shared_ptr<Customer> temp_customer,
                              ConfirmResult& result);

    static void PopupOnInputField(HintInputFieldsW_Popup& field, 
                                  bool* selected, const char* label);

    static void PopupOnInputField(HintInputFieldsW_PopupStr& field, bool* selected, const char* label);

private:
    static void CenterAlign();
};