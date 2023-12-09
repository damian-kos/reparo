#pragma once
#include "structs.h"
#include <functional>
#include <cmath>

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
    IMGUI_API void SetFonts();
    IMGUI_API void ViewCustomer(const Customer& customer, std::shared_ptr<Customer> t_customer);
    IMGUI_API void ViewRepair(const Repair& repair, 
                              std::shared_ptr<Repair> t_repair, 
                              std::shared_ptr<Customer> t_customer);

    IMGUI_API void SeparatorDecorator(const char* label, bool decorated);
    IMGUI_API void SeparatorTextAlignC(const char* label, int width);
    IMGUI_API void SeparatorTextAlignR(const char* label);
    IMGUI_API bool CenteredButton(const char* label, const ImVec2& size_arg, std::string test);
    IMGUI_API void CenteredText(const char* label, const ImVec2& size_arg);
    IMGUI_API void InputTextWithHintExt(const char* label, const char* hint, HintInputField& field, std::function<bool()> validation_function, bool* feedback);
    IMGUI_API void GetFontV(ImFont* font_t);
    IMGUI_API bool ButtonScaled(const char* label, float font_size, ImVec2 button_size);

    IMGUI_API void InputTextWithPopup(const char* label, const char* hint, HintInputFieldsW_Popup& field, std::function<bool()> validation_function, bool* selection, HintInputFieldsW_Popup* rel_field, bool* feedback);
        
} // namespace ImGui



#endif
