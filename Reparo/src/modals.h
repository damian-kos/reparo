#pragma once
#include <unordered_map>
#include "imgui.h"
#include "structs.h"
#include "imgui_viewer.h"
#include "enums.h"
class ModalController {
public:
    void RenderModal(const char* modal_title);
    void SubmitConfirm(const char* modal_title, Customer& customer, ConfirmResult&);
    void SubmitConfirm(const char* modal_title, Repair& repair, ConfirmResult& result);
    static void PopupOnInputField(HintInputFieldsW_Popup& field, bool* selected, const char* label);

private:
    void CenterAlign();

};