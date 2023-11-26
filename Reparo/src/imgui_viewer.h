#pragma once
#include <functional>

#ifndef IMGUIVIEWER_H_
#define IMGUIVIEWER_H_

#ifndef IMGUI_API
#include <imgui.h>
#endif //IMGUI_API

// USAGE
/*
#include "imguidatechooser.h"
//#include <time.h>   // mandatory when implementing ImGui::TestDateChooser() yourself in your code

// inside a ImGui::Window:
ImGui::TestDateChooser();
*/

struct Customer;  // defined in "structs.h"

namespace ImGui {
    IMGUI_API void ViewCustomer(const Customer& customer);
    IMGUI_API void ViewRepair(const Repair& repair);

    IMGUI_API void SeparatorDecorator(const char* label, bool decorated);
    IMGUI_API void InputTextWithHintExt(const char* label, const char* hint, HintInputField& field, std::function<bool()> validation_function, bool* feedback);


    IMGUI_API void InputTextWithPopup(const char* label, const char* hint, HintInputFieldsW_Popup& field, std::function<bool()> validation_function, bool* selection, HintInputFieldsW_Popup* rel_field, bool* feedback);
        
} // namespace ImGui



#endif
