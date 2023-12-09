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

    virtual void Render();
    void RepairsToTable(RepairsSort& repairs, int& state, bool sorting = false);
    void RunModal();
    std::shared_ptr<EditRepair> GetEditRepair();
    std::vector<std::string> names;
    bool* opened;
private:
    RepairsSort all_repairs_of_state;
    int prev_chosen_tab;
    int curr_chosen_tab;
    Repair repair_to_init;
    bool modal;
    int modal_on_id;
    ConfirmResult deletion = ConfirmResult::CONIFRM_IDLE;
    std::shared_ptr<EditRepair> edit_repair;
};