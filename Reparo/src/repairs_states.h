#pragma once

#include "imgui.h"
#include "imgui_helper.h"
#include "sql_queries.h"

class Repairs {
public:
    void AllToDoRepairs();
    std::vector<Repair> to_do_repairs;
    bool to_do_retreived;
private:

    SQLQuery sql;
};

class RepairMenu {
public:
    void RepairMainWin();
    Repairs repairs;

private:
    ImGuiHelper helper;
};