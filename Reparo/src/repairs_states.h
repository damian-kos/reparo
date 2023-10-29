#pragma once

#include "imgui.h"
#include "imgui_helper.h"
#include "sql_queries.h"
#include "add_repair.h"


class Repairs {
public:

    void RepairsSelector(int selection, bool& repair_update_window_on);
    template <RepairState state>
    std::vector<Repair>& GetRepairs();
    ImGuiHelper helper;
    int sel;
    int current_selection = -1;
    int previous_selection = -1;
    Repair device;
private:
    std::vector<Repair> curr_repairs;

    SQLQuery sql;
};

class RepairMenu {
public:
    void RepairMainWin();
    void UpdateRepairWin();
    Repairs repairs;
    bool repair_update_window_on;
    AddRepair add_repair;

private:
};