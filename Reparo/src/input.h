#pragma once

#include "imgui.h"
//#include "inputs_handler.h"
#include "search.h"
#include <string>

// Define InputField struct here
struct InputField {
    const char* label;
    const char* hint;
    char* buffer;
    int bufferSize;
    ImGuiInputTextFlags flags;
};

class CustomerInputWindow {


public:
    void Render();


};
