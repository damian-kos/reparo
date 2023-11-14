#pragma once
#include "imgui.h"
#include "structs.h"
#include "enums.h"
class ModalController {
public:
    void RenderModal(const char* modal_title);
    void SubmitConfirm(const char* modal_title, Customer& customer, ConfirmResult&);
    void PopupOnInputField(HintInputFieldsW_Popup& field, const char* label);
private:
    void CenterAlign();

};