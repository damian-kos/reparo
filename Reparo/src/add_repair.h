#pragma once

//#include "imgui.h"
#include "search.h"
#include "parts_stock_window.h"

void SearchForCustomers();
void SearchForByPhone();
void ComboModels(Part& device);
void ComboCategories(Part& device);
void ComboColors(Part& device);
void ComboStates(RepairData& state);

class AddRepair {
public:
    void AddRepairWindow();
};