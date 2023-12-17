#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <memory>
#include "insert_repair.h"

class EditRepair : public InsertRepair, public std::enable_shared_from_this<EditRepair> {
public:
    //EditRepair();
    EditRepair(Repair& repair, int passed_repair_id);
    ~EditRepair();
public:
    static bool* show_repair;
    void Render() override;
    void StateSection() override;
    Repair InitRepair() override;
    void TestButton() override;
    void UpdateSummary();
    static void Set(std::shared_ptr<EditRepair> edit_repair);
    bool CustomerModified();
    bool RepairModified();
    std::shared_ptr<Customer> t_customer = nullptr;
    std::shared_ptr<Repair> t_repair = nullptr;
    std::string selected_state;
    bool observers_attached = false;

private:
  static std::shared_ptr<EditRepair> instance;

private:
    std::vector<RepairUpdates> updates;
    int repair_id;
    bool RepairValidated() override;
    void InsertRepairButton() override;
    void RunModal(Repair& repair) override;
    static int instance_count;

    std::string str_date = "";

public:
  static std::shared_ptr<EditRepair> Get();
};