#pragma once

#include <iostream>
#include <cmath>
#include "insert_repair.h"

class EditRepair : public InsertRepair {
public:
    //EditRepair();
    EditRepair(Repair& repair, int passed_repair_id);
    ~EditRepair();

    void StateSection() override;
    void TestButton() override;
    bool CustomerModified();
    bool RepairModified();
    static bool* show_repair;
private:
  
    int repair_id;
    void InsertRepairButton() override;
    void RunModal(Repair& repair) override;
    static int instance_count;
};