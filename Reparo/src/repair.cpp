#include "repair.h"
#include <fstream>
//#include <nlohmann/json.hpp>
#include <imgui.h>

using json = nlohmann::json;
static std::vector<std::string> itemsArray; // Declare itemsArray as a global vector.
static std::vector<std::string> subitemsArray; // Declare subitemsArray to store sub-item choices.
static int item_current_idx = -1; // Here we store our selection data as an index.
static int subitem_current_idx = -1; // Here we store the sub-item selection data as an index.

// Function to read and parse the JSON file
bool ReadJSONFile(const std::string& filename, json& j) {
    try {
        std::ifstream file(filename);
        file >> j;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing JSON file: " << e.what() << std::endl;
        return false;
    }
}

std::vector<std::string> GenerateItemsArray(const json& jsonData) {
    std::vector<std::string> itemsArray;

    for (auto it = jsonData.begin(); it != jsonData.end(); ++it) {
        itemsArray.push_back(it.key());
    }

    return itemsArray;
}

std::vector<std::string> GenerateSubitemsArray(const json& jsonData, const std::string& selectedItem) {
    std::vector<std::string> subitemsArray;

    if (jsonData.find(selectedItem) != jsonData.end()) {
        const json& subitems = jsonData[selectedItem];

        for (auto it = subitems.begin(); it != subitems.end(); ++it) {
            subitemsArray.push_back(it.key());
        }
    }

    return subitemsArray;
}

void AddRepairWindow() {
    json jsonData;
    if (ReadJSONFile("devices.json", jsonData)) {
        // Generate the items array
        itemsArray = GenerateItemsArray(jsonData);

        // ImGui code for your window...
        ImGui::Begin("Add repair");

        if (ImGui::BeginListBox("##Brand")) {
            for (int n = 0; n < itemsArray.size(); n++) {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(itemsArray[n].c_str(), is_selected))
                    item_current_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        
        ImGui::SameLine();

        if (item_current_idx != -1) {
            // Show sub-items based on the selected item
            const std::string selectedItem = itemsArray[item_current_idx];
            subitemsArray = GenerateSubitemsArray(jsonData, selectedItem);

        }
        if (ImGui::BeginListBox("##Subitem")) {
            for (int n = 0; n < subitemsArray.size(); n++) {
                const bool is_selected = (subitem_current_idx == n);
                if (ImGui::Selectable(subitemsArray[n].c_str(), is_selected))
                    subitem_current_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }


        ImGui::End(); // End your ImGui window
    }

}