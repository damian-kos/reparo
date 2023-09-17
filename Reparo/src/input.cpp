#include "imgui.h"
#include "debug_window.h"
#include "inputs_handler.h" 
#include "input.h"
#include "search.h"
#include <fstream>

static char name[128] = "";
static char surname[128] = "";
static char email[128] = "";
static char phoneNumber[128] = "";



InputField inputFields[] = {
    {"##Name", "Name..", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_None},
    {"##Surname", "Surname..", surname, IM_ARRAYSIZE(surname), ImGuiInputTextFlags_None},
    {"##Email", "Email..", email, IM_ARRAYSIZE(email), ImGuiInputTextFlags_None},
    {"##PhoneNumber", "Phone Number..", phoneNumber, IM_ARRAYSIZE(phoneNumber), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank}
};

void DebugLog() {
    // Concatenate the values from the input fields
    std::string concatenatedText = "Name: " + std::string(name) +
        "\nSurname: " + std::string(surname) +
        "\nEmail: " + std::string(email) +
        "\nPhone Number: " + std::string(phoneNumber);

    // Pass the concatenated text to SetDebugWindowText
    SetDebugWindowText(concatenatedText.c_str());
}


void CustomerDetailsInputWindow() {
    ImGui::Begin("Customer Details");

    // Create input fields
    for (const InputField& field : inputFields) {
        ImGui::InputTextWithHint(field.label, field.hint, field.buffer, field.bufferSize, field.flags);
    }
    // Pass filled inputs fields to handler.
    if (ImGui::Button("Submit Customer Details")) {
        DebugLog();
        InputsHandler::HandleInputsFromFields(inputFields, sizeof(inputFields) / sizeof(inputFields[0]));

        // Clear the input fields using memset
        for (InputField& field : inputFields) {
            memset(field.buffer, 0, field.bufferSize); // Set all characters to null (clear the buffer)
        }
    }
    SearchField();

    ImGui::End();
}
