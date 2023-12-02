#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <memory>
#include "insert_repair.h"

class EditRepair : public InsertRepair {
public:
    //EditRepair();
    EditRepair(Repair& repair, int passed_repair_id);
    ~EditRepair();

    void Render() override;
    void StateSection() override;
    Repair InitRepair() override;
    void TestButton() override;
    void UpdateSummary();
    bool CustomerModified();
    bool RepairModified();
    static bool* show_repair;
    std::shared_ptr<Customer> t_customer = nullptr;
    std::shared_ptr<Repair> t_repair = nullptr;

    std::string selected_state;
private:
    std::vector<RepairUpdates> updates;
    int repair_id;
    bool RepairValidated() override;

    void InsertRepairButton() override;
    void RunModal(Repair& repair) override;
    static int instance_count;
};