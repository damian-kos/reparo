#pragma once

#include "imgui.h"
#include "search.h"
#include <string>
#include "error_handler.h"
#include "inputs_handler.h"
#include "input_field.h"
#include "sql_queries.h"



class CustomerInputWindow {

public:
    ModalController modalController;
    void Render();
    void CreateInputFields();
    void Submit();
    Customer customer;
    void SearchForExsitingCustomers();

private:
    bool inputError = false;

};
