#pragma once

#include <string>
#include "imgui.h"
#include "structs.h"


class ModalController {
public:
    void RenderErrorModal(const char* error_name);
    void GetErrorState(const char* error_name, const char* errorMessage, int& state);
    void RepairConfirmation(const char* error_name, Repair& repair, RepairSubmissionState&);
    void RepairDetails(Repair& repair);
    void CustomerDetails(Customer&);
};