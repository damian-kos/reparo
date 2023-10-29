#pragma once
#include "imgui.h"
#include "structs.h"
#include "edit_customer.h"
#include <vector>
#include <string>


class ImGuiHelper {
public:
    void PopulateListBox(const char* label, PartData& part);
    void PopulateListBoxMulti(const char* label,PartQualityData& quality);
    void PartTableStockWindow(Part& part);
    bool TableBegin(const char* label, int columns, std::vector<std::string> columns_name, int i, ImGuiTableFlags flags);
    void TablesColumnsText(int column, std::string text);
    void ComboForDevice(const char* label, PartData& device);
    void RepairStatesTable(std::vector<Repair>& repairs, int& selected, bool& update_repair);

};

class ImGuiDecorator {
public:
    ImGuiDecorator(bool initialTestValue) : test_(initialTestValue) { }

    void DecoratedSeparatorText(const char* text) {
        if (test_) {
            ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        }
        ImGui::SeparatorText(text);

        ImGui::PopStyleColor(1);
    }

    void SetTestValue(bool newTestValue) {
        test_ = newTestValue;
    }
private:
    bool test_;
};
