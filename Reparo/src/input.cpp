#include "input.h"
#include "inputs_handler.h"


    char name[128] = "";
    char surname[128] = "";
    char email[128] = "";
    char phoneNumber[128] = "";

    InputField inputFields[4] = {
        {"##Name", "Name..", name, IM_ARRAYSIZE(name), ImGuiInputTextFlags_None},
        {"##Surname", "Surname..", surname, IM_ARRAYSIZE(surname), ImGuiInputTextFlags_None},
        {"##Email", "Email..", email, IM_ARRAYSIZE(email), ImGuiInputTextFlags_None},
        {"##PhoneNumber", "Phone Number..", phoneNumber, IM_ARRAYSIZE(phoneNumber), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank}
    };
void CustomerInputWindow::Render() {
    
        ImGui::Begin("Add customer");
        ImGui::PushItemWidth(-1);

        int phoneNumberIndex = -1; // Initialize to an invalid index
        char xd[128] = "";



        // Create input fields
        for (int i = 0; i < sizeof(inputFields) / sizeof(inputFields[0]); ++i) {
            ImGui::InputTextWithHint(inputFields[i].label, inputFields[i].hint, inputFields[i].buffer, inputFields[i].bufferSize, inputFields[i].flags);

            if (std::strcmp(inputFields[i].label, "##PhoneNumber") == 0) {
                phoneNumberIndex = i; // Store the index of the phoneNumber field
            }
        }

        ImGui::Spacing();

        // Pass filled input fields to the handler.
        if (ImGui::Button("Submit Customer Details")) {
          
            InputsHandler::HandleInputsFromFields(inputFields, sizeof(inputFields) / sizeof(inputFields[0]));

            // Clear the input fields using memset
            for (InputField& field : inputFields) {
                memset(field.buffer, 0, field.bufferSize); // Set all characters to null (clear the buffer)
            }
        }

        ImGui::Spacing();
        if (phoneNumberIndex != -1) {
            SearchField(inputFields[phoneNumberIndex].buffer);
        }
        ImGui::End();
    }




