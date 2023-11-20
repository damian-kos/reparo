#pragma once
#include "imgui.h"
#include "structs.h"
#include "enums.h"
class ModalController {
public:
    void RenderModal(const char* modal_title);
    void SubmitConfirm(const char* modal_title, Customer& customer, ConfirmResult&);
    void SubmitConfirm(const char* modal_title, Repair& repair, ConfirmResult& result);
    void PopupOnInputField(HintInputFieldsW_Popup& field, const char* label);
    void PopupOnInputField(HintInputFieldsW_Popup& field, const char* label, bool& selected);
private:
    void CenterAlign();

};