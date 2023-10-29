#pragma once

#include "imgui.h"
#include "imgui_helper.h"
#include "sql_queries.h"


class Repairs {
public:
    void RetreiveToDoRepairs();
    void RetreiveProcessingRepairs();
    void RetreiveWarrantyRepairs();
    void RetreiveAwaitingRepairs();
    void RepairsSelector(int selection);
    RepairsVector to_do;
    RepairsVector processing;
    RepairsVector warranty;
    RepairsVector awaiting;
    ImGuiHelper helper;


private:

    SQLQuery sql;
};

class RepairMenu {
public:
    void RepairMainWin();
    Repairs repairs;

private:
};