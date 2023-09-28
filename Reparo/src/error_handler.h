#pragma once

#include <string>
#include "imgui.h"

class ModalController {
public:
    bool isModalOpen = false;

    void ShowModal() {
        isModalOpen = true;
    }

    void CloseModal() {
        isModalOpen = false;
    }

    void RenderErrorModal(const char* errorName);

    void GetErrorState(const char* errorName, const char* errorMessage);
};