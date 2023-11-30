#pragma once

#include <iostream>
#include <memory>

#include "imgui.h"
#include "database.h"
#include "edit_repair.h"

class RepairsView {
public:
    RepairsView();

    ~RepairsView();

    void Render();
    void RepairsToTable(RepairsSort& repairs);
    std::shared_ptr<EditRepair> GetEditRepair();
private:
    RepairsSort repairs;
    int prev_chosen_tab;
    int curr_chosen_tab;
    Repair repair_to_init;
    std::shared_ptr<EditRepair> edit_repair;
};