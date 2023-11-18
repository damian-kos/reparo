#pragma once

#include <iostream>

#include "imgui.h"
#include "database.h"

class RepairsView {
public:
    RepairsView();

    ~RepairsView();

    void Render();
    void RepairsToTable(const std::map<int, Repair>& repairs);
private:
    Database db;
     std::map<int, Repair> repairs;
     int prev_chosen_tab;
     int curr_chosen_tab;
};