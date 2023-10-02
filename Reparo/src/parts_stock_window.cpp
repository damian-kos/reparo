//#include "input_field.h"
#include <vector>
#include <string>
#include <iostream>
#include "parts_stock_window.h"
#include <unordered_map>

char partQuery[128] = "";
static int current_brand_id = -1; // Initialize current_brand_id
static int previorus_brand_id = -1;
static std::vector<std::string> brands; // Change the vector type to `const char*`

static int current_model_id = -1; // Initialize current_brand_id
static std::vector<std::string> models; // Change the vector type to `const char*`

static int current_category_id = -1; // Initialize current_brand_id
static std::vector<std::string> categories; // Change the vector type to `const char*`

static int current_color_id = -1; // Initialize current_brand_id
static std::vector<std::string> colors; // Change the vector type to `const char*`

static bool selected_qualities[20] = {}; // Initialize current_brand_id
static std::vector<std::string> qualities; // Change the vector type to `const char*`
static std::vector<int> order;


bool brands_retreived = false;
bool qualities_populated = false;

std::string text;

void PartsStockWindow::Render() {
  
    ImGui::Begin("Add part to stock");
    if (ImGui::BeginTable("Too add", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("Brand");
        ImGui::TableSetupColumn("Model");
        ImGui::TableSetupColumn("Category");
        ImGui::TableSetupColumn("Color(if any)");
        ImGui::TableSetupColumn("Quality(if any)");
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        if (current_brand_id != -1) {
            ImGui::Text(brands[current_brand_id].c_str());
        }
        ImGui::TableSetColumnIndex(1);
        if (current_model_id != -1) {
            ImGui::Text(models[current_model_id].c_str());
        }
        ImGui::TableSetColumnIndex(2);
        if (current_category_id != -1) {
            ImGui::Text(categories[current_category_id].c_str());
        }
        ImGui::TableSetColumnIndex(3);
        if (current_color_id != -1) {
            ImGui::Text(colors[current_color_id].c_str());
        }
        ImGui::TableSetColumnIndex(4);
        if (!qualities_populated) {
            if (order.size() == 0) {
                text = "";
            }
            else {
                text = "";
                for (size_t i = 0; i < order.size(); i++)
                {
                    text += qualities[order[i]] + " ";
                }
            }
            qualities_populated = true; 
        }
        ImGui::Text(text.c_str());

        
        ImGui::EndTable();
 
    }

    if (ImGui::Button("hash")) {
        std::cout << order.size() << "[";

        for (size_t i = 0; i < order.size(); i++) {
            std::cout << order[i];
            if (i < order.size() - 1) {
                std::cout << ", ";
            }
        }

        std::cout << "]" << std::endl;
    }

    ImGui::InputTextWithHint("##SearchPart", "Part to search... ", partQuery, IM_ARRAYSIZE(partQuery), ImGuiInputTextFlags_ReadOnly);

    ImGui::PushItemWidth(128);

    PopulateListBox("##Brand", brands, current_brand_id);
    ResetOnBrandChange();

    if (current_brand_id != -1) {
        GetModels();
    }

    PopulateListBox("##Models", models, current_model_id);

    if (current_model_id != -1) {
        GetCategories();
        if (current_category_id+1 == 1 || current_category_id + 1 == 4)  {
            GetColorsForModel();
        }
        else {
            colors.clear();
        }
    }

    PopulateListBox("##Categories", categories, current_category_id);
 
    PopulateListBox("##Colors", colors, current_color_id);

    if (current_category_id != -1) {
        GetQualities();
        PopulateListBoxMulti("##Qualities", qualities, selected_qualities);
    }

    ImGui::End();
}

void ResetOnBrandChange() {
    if (current_brand_id != previorus_brand_id) {
        std::cout << "RESET" << std::endl;
        previorus_brand_id = current_brand_id;
        current_model_id = -1;
        current_category_id = -1;
        current_color_id = -1;
        models.clear();
        categories.clear();
        colors.clear();
        order.clear();
        text = "";
        qualities.clear(); 
        for (bool& value : selected_qualities) { value = false; }
    }
}

void PopulateListBox(const char* label, std::vector<std::string>& vector, int& selectable) {
    if (ImGui::BeginListBox(label)) {
        for (int n = 0; n < vector.size(); n++) {
            const bool is_selected = (selectable == n);
            if (ImGui::Selectable(vector[n].c_str(), is_selected)) // Removed .c_str()
                selectable = n;
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
    ImGui::SameLine();
}

void PopulateListBoxMulti(const char* label, std::vector<std::string>& vector, bool(&selectables)[20]) {
    if (ImGui::BeginListBox(label)) {
      int nextAvailableKey = 1; // Initialize the next available key
      for (int n = 0; n < vector.size(); n++) {
        if (ImGui::Selectable(vector[n].c_str(), selectables[n])) { // Removed .c_str()
                selectables[n] = !selectables[n];
                if (selectables[n]) {
                    order.push_back(n);
                    qualities_populated = false;
                }
                else {
                    auto it = std::find(order.begin(), order.end(), n);
                    // Check if the index was found before proceeding
                    if (it != order.end()) {
                        // Erase the index from the selected list
                        order.erase(it);
                        std::cout << "ERASED " << it._Unwrapped() << std::endl;
                    }
                    qualities_populated = false;

                }
            }
        }
        ImGui::EndListBox();
    }
    ImGui::SameLine();
}

void PartsStockWindow::Query(const char* query, std::vector<std::string>& vector) {

    partsStock.OpenPartsStockDb();
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(partsStock.db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(partsStock.db) << std::endl;
    }
    else {
        // Execute the query and populate the `brands` vector
        vector.clear(); // Clear the vector if needed

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* record = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            vector.emplace_back(record);
        }
        // Finalize the statement
        sqlite3_finalize(stmt);
        sqlite3_close(partsStock.db);
    }
}

void PartsStockWindow::GetBrands() {
    if (!brands_retreived) {

        const char* brandsQuery = "SELECT brand FROM brands";
        Query(brandsQuery, brands);

        sqlite3_close(partsStock.db);
        brands_retreived = true;

    }
}

void PartsStockWindow::QueryRelationalTables(const char* query, std::vector<std::string>& vector, int id) {

    partsStock.OpenPartsStockDb();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(partsStock.db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind the id parameter to the placeholder in query
        sqlite3_bind_int(stmt, 1, id);
        vector.clear();
        // Execute the query
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Retrieve the model data from the result set
            const char* record = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            vector.emplace_back(record);
        }

        // Finalize the statement after use
        sqlite3_finalize(stmt);
    }
    else {
        std::cerr << "Error preparing SQL statement: SPECIFIC " << sqlite3_errmsg(partsStock.db) << std::endl;
    }
    sqlite3_close(partsStock.db);

}

void PartsStockWindow::GetModels() {

    const char* currentBrand = brands[current_brand_id].c_str();
    int brand_id = GetIdForValue("brands", "brand", currentBrand);

    const char* brandsQuery = "SELECT model FROM models WHERE brand_id = ?";
    QueryRelationalTables(brandsQuery, models, brand_id);

}

void PartsStockWindow::GetCategories() {

    const char* categoriesQuery = "SELECT category FROM categories";
    Query(categoriesQuery, categories);

}

void PartsStockWindow::GetQualities() {

    const char* qualitiesQuery = "SELECT quality FROM qualities";
    Query(qualitiesQuery, qualities);

}


void PartsStockWindow::GetColorsForModel() {

    const char* currentModel = models[current_model_id].c_str();
    int model_id = GetIdForValue("models", "model", currentModel);

    const char* colorsQuery = "SELECT color FROM colors WHERE color_id IN (SELECT color_id FROM model_color WHERE model_id = ?)";

    QueryRelationalTables(colorsQuery, colors, model_id);
    
}

int PartsStockWindow::GetIdForValue(const char* tableName, const char* columnName, const char* searchValue) {
    
    partsStock.OpenPartsStockDb();

    // Construct the SQL query as a string
    std::string sql = "SELECT " + std::string(columnName)+"_id FROM " + std::string(tableName) + " WHERE " + std::string(columnName) + " = ? ";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(partsStock.db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(partsStock.db) << std::endl;
        sqlite3_close(partsStock.db);
        return -1; // Return an error code or handle the error appropriately
    }

    // Bind the search value parameter to the placeholder
    rc = sqlite3_bind_text(stmt, 1, searchValue, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding search value parameter: " << sqlite3_errmsg(partsStock.db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(partsStock.db);
        return -1; // Return an error code or handle the error appropriately
    }

    int id = -1; // Default value if the value is not found

    // Execute the query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Retrieve the ID from the result set
        id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(partsStock.db);

    return id;
}