#pragma once

#include "sql_queries.h"
#include "error_handler.h"
#include "structs.h"
#include <iostream>

class CustomerEditWindow {

public:
    ModalController modalController;

    void SetCustomerToEdit(Customer* customerData, int id);
    void Render();
    void DataToFields();
    void SetShouldRender(bool);
    bool GetShouldRender();
};
