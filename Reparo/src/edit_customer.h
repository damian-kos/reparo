#pragma once

#include "customer_data.h"
#include "inputs_handler.h"
#include "customer_input_window.h"
#include "error_handler.h"

class CustomerEditWindow {

public:
    ModalController modalController;

    void SetCustomerToEdit(json* customerData);
    void Render();
    void DataToFields();
    void SetShouldRender(bool);
    bool GetShouldRender();
};
