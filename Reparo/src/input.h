#pragma once
// input.h

#ifndef INPUT_H
#define INPUT_H

#include "imgui.h"
#include <iostream>


struct InputField {
    const char* label;
    const char* hint;
    char* buffer;
    size_t bufferSize;
    ImGuiInputTextFlags flags;
};

// Declare the input handling functions
void CustomerDetailsInputWindow();




#endif // INPUT_H
