#pragma once

#include <iostream>

#include "insert_repair.h"

class EditRepair : public InsertRepair {
public:
    EditRepair();
    EditRepair(Repair& repair);
    ~EditRepair();

    void StateSection() override;

    static bool* show_repair;
private:
    static int instance_count;
};