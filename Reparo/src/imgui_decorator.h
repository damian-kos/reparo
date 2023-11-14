#pragma once
#include "imgui.h"
class ImGuiDecorator {
public:
    ImGuiDecorator();
    void DecorateSeparatorText(const char* text);
    void SetTestValue(bool new_value);

private:
    bool decorate = false;
};