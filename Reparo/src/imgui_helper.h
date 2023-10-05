#pragma once
#include "imgui.h"
#include "input_field.h"
#include <vector>
#include <string>

class ImGuiHelper {
public:
    void PopulateListBox(const char* label, std::vector<std::string>& vector, int& selectable);
    void PopulateListBoxMulti(const char* label,PartQualityData& quality);
    //void PartTable(int columns, ):

};

