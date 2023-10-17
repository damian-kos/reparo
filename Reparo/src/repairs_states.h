#pragma once

#include "imgui.h"
#include "imgui_helper.h"
#include "sql_queries.h"

class Repairs {
public:
    void AllToDoRepairs();

private:
    Repair repair;
    SQLQuery sql;
};

class RepairMenu {
public:
    void RepairMainWin();
};