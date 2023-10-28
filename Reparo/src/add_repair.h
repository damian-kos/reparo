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
    void SubmitRepairButton();
    std::string error_message;
    //ModalController modalController;

private:
    char searchQuery[128] = "";
    char notes[512] = "";
    char notes_hidden[512] = "";
    bool device_validation;

private: //Structs
    PopupInput pop_model;
    PopupInput pop_category;
    PopupInput pop_color;
    Repair device;
private:
    //Search search;
    //PartsStockWindow parts;
    RepairSubmissionState repair_submission = 0;
    ImGuiDecorator decorator;
    CustomerInputWindow customerInput;
    SQLQuery sql;
    bool model_validated;
    bool price_validation;
    std::string device_validate_feedback = "";
    ModalController modalController;

private:
    void SubmitRepair();
    void SubmissionConfirmed();
    bool CanSubmitRepair();
    void Models();
    void UpdateDeviceValidationFeedback();
    void Categories();
    void Colors();
    void SearchForByPhone();
    void SearchForCustomers();
    void Combo(Repair& device, std::string label, PartData& attribute);
    void GetIDs();

};