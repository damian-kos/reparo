#pragma once
// add_repair.h
#include "imgui_helper.h"
#include "search.h"
#include "parts_stock_window.h"
#include "customer_fields.h"
#include "error_handler.h"
#include "sql_queries.h"
#include "customer_input_window.h"



class AddRepair {
public:
    AddRepair() : customerInput("some text", CustomerInputFlags_NoSurnameField), decorator(false) {
    }
   
    void AddRepairWindow();
    std::string error_message;
    ModalController modalController;

private:
    char searchQuery[128] = "";
    char notes[512] = "";
    char notes_hidden[512] = "";
    bool device_validation;

private:
    PopupInput pop_model;
    PopupInput pop_category;
    PopupInput pop_color;
    Search search;
    ImGuiDecorator decorator;
    CustomerInputWindow customerInput;
    SQLQuery sql;
    Repair device;

private:
    void CheckDeviceInputs();
    void SubmitRepair();
    void Models();
    void Categories();
    void Colors();
    void SearchForByPhone();
    void SearchForCustomers();
    void Combo(Repair& device, std::string label, PartData& attribute);
    void GetIDs();

};