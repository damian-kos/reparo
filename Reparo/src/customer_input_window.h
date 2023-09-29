#pragma once

#include "imgui.h"
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

    void CreateInputFields();

    void Submit();

    void SearchForCustomers();


};
