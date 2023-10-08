#pragma once
#include "imgui.h"
#include "structs.h"
#include "edit_customer.h"
#include <vector>
#include <string>

class ImGuiHelper {
public:
    void PopulateListBox(const char* label, std::vector<std::string>& vector, int& selectable, std::string& text);
    void PopulateListBoxMulti(const char* label,PartQualityData& quality);
    void PartTableStockWindow(Part& part);
    void TableBegin(const char* label, int columns, std::vector<std::string> columns_name, ImGuiTableFlags flags);
    void TablesColumnsText(int column, std::string text);
    void PopupOnItemOfTable(const char* text, Customer& val, int id);
};

