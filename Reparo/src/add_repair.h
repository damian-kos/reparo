#pragma once

//#include "imgui.h"
#include "search.h"
#include "parts_stock_window.h"
#include "customer_fields.h"
#include "error_handler.h"
#include "sql_queries.h"
#include "customer_input_window.h"


void Combo(Repair& device, std::string label, PartData& attribute);


class AddRepair {
public:
    void Models();
    void Categories();
    void Colors();

    PopupInput pop_model;
    PopupInput pop_category;
    PopupInput pop_color;
    
    std::string error_message;
    void AddRepairWindow();

    void SubmitRepair();
    void ProcessSubmission();
    int submit;
    ModalController modalController;

private:
    Search search;
    CustomerInputWindow customerInput;

    SQLQuery sql;
    void GetIDs();
    void SearchForByPhone();
    void SearchForCustomers();

};