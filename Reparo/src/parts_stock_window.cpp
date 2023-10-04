//#include "input_field.h"
#include <vector>
#include <string>
#include <iostream>
#include "parts_stock_window.h"
#include <unordered_map>
#include <set>
#include <sstream>

char partQuery[128] = "";

static int previous_brand_id = -1;

static bool selected_qualities[20] = {}; 
static std::vector<std::string> qualities; 
static std::vector<int> order;

std::string qualityString = "";
bool qualities_retreived = false;

std::string text;

void PartsStockWindow::Render() {
  
    ImGui::Begin("Add part to stock");
    ImGui::InputTextWithHint(
        "##SearchPart", "Part to search... ", partQuery, 
        IM_ARRAYSIZE(partQuery), ImGuiInputTextFlags_ReadOnly);
    if (ImGui::BeginTable("Too add", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("Brand");
        ImGui::TableSetupColumn("Model");
        ImGui::TableSetupColumn("Category");
        ImGui::TableSetupColumn("Color(if any)");
        ImGui::TableSetupColumn("Quality(if any)");
        ImGui::TableHeadersRow();
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (brand.current_id != -1) {
            ImGui::Text(brand.data[brand.current_id].c_str());
        }
        ImGui::TableSetColumnIndex(1);
        if (model.current_id != -1) {
            ImGui::Text(model.data[model.current_id].c_str());
        }
        ImGui::TableSetColumnIndex(2);
        if (category.current_id != -1) {
            ImGui::Text(category.data[category.current_id].c_str());
        }
        ImGui::TableSetColumnIndex(3);
        if (color.current_id != -1) {
            ImGui::Text(color.data[color.current_id].c_str());
        }
        ImGui::TableSetColumnIndex(4);
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
        ImGui::Text(text.c_str());
        ImGui::EndTable();
 
    }

    ImGui::PushItemWidth(128);

    PopulateListBox("##Brand", brand.data, brand.current_id);
    ResetOnBrandChange();

    if (brand.current_id != -1) {
        GetModels();
    }

    PopulateListBox("##Models", model.data, model.current_id);

    if (model.current_id != -1) {
        GetCategories();
        if (category.current_id+1 == 1 || category.current_id + 1 == 4)  {


            GetColorsForModel();
            std::cout << color.data.size() << "Size " << std::endl;

        }
        else {
            color.current_id = -1;
            color.data.clear();
        }
    }

    PopulateListBox("##Categories", category.data, category.current_id);

    PopulateListBox("##Colors", color.data, color.current_id);

    if (category.current_id != -1) {
        GetQualities();
        PopulateListBoxMulti("##Qualities", qualities, selected_qualities);
    }
    if (ImGui::Button("Add part to stock..")) {
        AddPart();
    }
    ImGui::End();
}


void PartsStockWindow::AddPart()
{
    if (brand.current_id == -1 || model.current_id == -1 || category.current_id == -1) { return; }

    int rowToUpdate = SearchForSimilarRecords();
    if (rowToUpdate > 0) {
        partsStock.OpenPartsStockDb();
        std::string updateQuery = "UPDATE parts SET quantity = quantity + 1 WHERE part_id = ?";
        sqlite3_stmt* updateStmt;
        if (sqlite3_prepare_v2(partsStock.db, updateQuery.c_str(), -1, &updateStmt, NULL) == SQLITE_OK) {
            // Bind the parameter values to the placeholders in the query
            sqlite3_bind_int(updateStmt, 1, rowToUpdate);

            // Execute the update query
            int rc = sqlite3_step(updateStmt);
            if (rc == SQLITE_DONE) {
                // Update successful
                std::cout << "Quantity increased by 1 for matching record." << std::endl;
            }
            else {
                std::cerr << "Error updating record: " << sqlite3_errmsg(partsStock.db) << std::endl;
            }
            // Finalize the update statement
            sqlite3_finalize(updateStmt);
            sqlite3_close(partsStock.db);
        }
        else {
            std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(partsStock.db) << std::endl;
        }
        return;
    }
    else {
        partsStock.OpenPartsStockDb();

        //Construct the SQL query to insert data into the 'parts' table
        std::string sqlQuery = "INSERT INTO parts (model_id, brand_id, category_id, color, quality, quantity) VALUES (";
        sqlQuery += std::to_string(model.current_id + 1) + ", ";
        sqlQuery += std::to_string(brand.current_id + 1) + ", ";
        sqlQuery += std::to_string(category.current_id + 1) + ", ";
        sqlQuery += std::to_string(color.current_id + 1) + ", ";
        sqlQuery += "'" + qualityString + "', "; // Assuming quality is TEXT
        sqlQuery += "1)"; // Assuming quantity is initialized to 0
        int rc = sqlite3_exec(partsStock.db, sqlQuery.c_str(), 0, 0, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(partsStock.db));
        }
        sqlite3_close(partsStock.db);

    }
}

std::set<std::string> TokenizeAndStore(const std::string& input) {
    std::set<std::string> words;
    std::istringstream iss(input);
    std::string word;

    while (iss >> word) {
        words.insert(word);
    }
    return words;
}

int PartsStockWindow::SearchForSimilarRecords() {
    partsStock.OpenPartsStockDb();
    if (order.size() == 0) {
        qualityString = "";
    }
    else {
        qualityString = "";
        for (size_t i = 0; i < order.size(); i++)
        {
            qualityString += qualities[order[i]] + " ";
        }
    }
    std::set<std::string> words1 = TokenizeAndStore(qualityString);
    const char* sqlQuery = "SELECT part_id, quality FROM parts WHERE brand_id = ? AND model_id = ? AND category_id = ? AND color = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(partsStock.db, sqlQuery, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the parameter values to the placeholders in the query
        int brandId = brand.current_id + 1;
        int modelId = model.current_id + 1;
        int categoryId = category.current_id + 1;
        int colorId = color.current_id + 1;

        sqlite3_bind_int(stmt, 1, brandId);
        sqlite3_bind_int(stmt, 2, modelId);
        sqlite3_bind_int(stmt, 3, categoryId);
        sqlite3_bind_int(stmt, 4, colorId);

        // Execute the query
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Retrieve data from the result set
            int partId = sqlite3_column_int(stmt, 0);
            const char* quality = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::set<std::string> words2 = TokenizeAndStore(quality);
            if (words1 == words2) {
                std::cout << "TRUE FOR: " << quality <<  " " << partId << std::endl;
            
                sqlite3_finalize(stmt);
                sqlite3_close(partsStock.db);
                return partId;
            }
            std::cout << " Quality: " << quality << std::endl;
        }

        // Finalize the statement
        sqlite3_finalize(stmt);
        sqlite3_close(partsStock.db);
        return 0;
    }
    else {
        // Handle the error
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(partsStock.db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(partsStock.db);
    return 0;

}


void PartsStockWindow::ResetOnBrandChange() {
    if (brand.current_id != previous_brand_id) {
        std::cout << "RESET" << std::endl;
        previous_brand_id = brand.current_id;
        model.current_id = -1;
        category.current_id = -1;
        color.current_id = -1;
        model.data.clear();
        category.data.clear();
        color.data.clear();
        order.clear();
        text = "";
        qualities.clear(); 
        for (bool& value : selected_qualities) { value = false; }
        qualityString = "";
        model.retreived = false;
        category.retreived = false;
        qualities_retreived = false;
        color.retreived = false;
    }
}

void PopulateListBox(const char* label, std::vector<std::string>& vector, int& selectable) {
    if (ImGui::BeginListBox(label)) {
        for (int n = 0; n < vector.size(); n++) {
            const bool is_selected = (selectable == n);
            if (ImGui::Selectable(vector[n].c_str(), is_selected))
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
                }
                else {
                    auto it = std::find(order.begin(), order.end(), n);
                    // Check if the index was found before proceeding
                    if (it != order.end()) {
                        // Erase the index from the selected list
                        order.erase(it);
                        std::cout << "ERASED " << it._Unwrapped() << std::endl;
                    }
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
        vector.clear(); // Clear the vector if needed

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* record = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            vector.emplace_back(record);
        }
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    sqlite3_close(partsStock.db);

}

void PartsStockWindow::GetBrands() {
    if (!brand.retreived) {

        const char* brandsQuery = "SELECT brand FROM brands";
        Query(brandsQuery, brand.data);

        sqlite3_close(partsStock.db);
        brand.retreived = true;

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
    if (!model.retreived) {

        const char* currentBrand = brand.data[brand.current_id].c_str();
        int brand_id = GetIdForValue("brands", "brand", currentBrand);
        const char* modelsQuery = "SELECT model FROM models WHERE brand_id = ?";
        QueryRelationalTables(modelsQuery, model.data, brand_id);
        model.retreived = true;
    }
}

void PartsStockWindow::GetCategories() {
    if (!category.retreived) {
        const char* categoriesQuery = "SELECT category FROM categories";
        Query(categoriesQuery, category.data);
        category.retreived = true;
    }
}

void PartsStockWindow::GetQualities() {
    if (!qualities_retreived) {
        const char* qualitiesQuery = "SELECT quality FROM qualities";
        Query(qualitiesQuery, qualities);
        qualities_retreived = true;
    }

}


void PartsStockWindow::GetColorsForModel() {
    if (!color.retreived) {
        const char* currentModel = model.data[model.current_id].c_str();
        int model_id = GetIdForValue("models", "model", currentModel);
        const char* colorsQuery = "SELECT color FROM colors WHERE color_id IN (SELECT color_id FROM model_color WHERE model_id = ?)";
        QueryRelationalTables(colorsQuery, color.data, model_id);
        color.retreived = true;

    }
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