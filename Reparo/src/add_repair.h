#pragma once

//#include "imgui.h"
#include "search.h"
#include "parts_stock_window.h"
#include "customer_fields.h"
#include "error_handler.h"
#include "sql_queries.h"


void SearchForCustomers();
void SearchForByPhone();
void Combo(Repair& device, std::string label, PartData& attribute);

class AddRepair {
public:
    std::string errorMessage;

    void AddRepairWindow();
    void SubmitRepair(std::string& message);
    ModalController modalController;

};