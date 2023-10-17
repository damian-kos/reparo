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
void GetIDs(SQLQuery& sql);

class AddRepair {
public:
    void Models();
    void Categories();
    void Colors();
    SQLQuery sql;

    PopupInput pop_model;
    PopupInput pop_category;
    PopupInput pop_color;
    
    std::string errorMessage;
    void AddRepairWindow();
    void SubmitRepair(std::string& message);
    ModalController modalController;

};