#pragma once

#include "imgui.h"
#include "search.h"
#include <string>
#include "error_handler.h"


// Define InputField struct here
//struct InputField {
//    const char* label;
//    const char* hint;
//    char* buffer;
//    int bufferSize;
//    ImGuiInputTextFlags flags;
//};

class CustomerInputWindow {


public:
    ModalController modalController;

    void Render();

    void CreateInputFields();

    void Submit();

    void SearchForCustomers();

private:
    bool inputError = false;
};
