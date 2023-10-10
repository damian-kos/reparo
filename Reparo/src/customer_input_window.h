#pragma once

#include "imgui.h"
#include "search.h"
#include "error_handler.h"
#include "structs.h"
#include "sql_queries.h"
#include "customer_fields.h"

class CustomerInputWindow {

public:
    ModalController modalController;
    void Render();
    void CreateInputFields();
    void Submit();
    Customer customer;
    void PassSearchQuery();

private:
    bool inputError = false;
};
