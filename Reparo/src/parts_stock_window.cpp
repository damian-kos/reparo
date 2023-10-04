//#include "input_field.h
#include "parts_stock_window.h"
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <sstream>

char partQuery[128] = "";

static int previous_brand_id = -1;



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
        if (quality.order.size() == 0) {
            //text = "";
            quality.desc = "";
        }
        else {
            /*text = "";*/
            quality.desc = "";

            for (size_t i = 0; i < quality.order.size(); i++)
            {
                quality.desc+= quality.data[quality.order[i]] + " ";

                //text += quality.data[quality.order[i]] + " ";
            }
        }
        ImGui::Text(quality.desc.c_str());
        ImGui::EndTable();
 
    }

    ImGui::PushItemWidth(128);

    imguiHelper.PopulateListBox("##Brand", brand.data, brand.current_id);
    
    ResetOnBrandChange();

    if (brand.current_id != -1) {
        GetModels();
    }

    imguiHelper.PopulateListBox("##Models", model.data, model.current_id);

    if (model.current_id != -1) {
        GetCategories();
        if (category.current_id+1 == 1 || category.current_id + 1 == 4)  {

            GetColorsForModel();
        }
        else {
            color.current_id = -1;
            color.data.clear();
            color.retreived = false;
        }
    }

    imguiHelper.PopulateListBox("##Categories", category.data, category.current_id);

    imguiHelper.PopulateListBox("##Colors", color.data, color.current_id);

    if (category.current_id != -1) {
        GetQualities();
        imguiHelper.PopulateListBoxMulti("##Qualities", quality);
    }
    if (ImGui::Button("Add part to stock..")) {
        AddPart();
    }
    ImGui::End();
}


void PartsStockWindow::AddPart()
{
    if (brand.current_id == -1 || model.current_id == -1 || category.current_id == -1) { return; }
    int rowToUpdate = sqlQuery.SearchForSimilarRecords(brand, model, category, color, quality);
    
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
        std::string sqlInsert = "INSERT INTO parts (model_id, brand_id, category_id, color, quality, quantity) VALUES (";
        sqlInsert += std::to_string(model.current_id + 1) + ", ";
        sqlInsert += std::to_string(brand.current_id + 1) + ", ";
        sqlInsert += std::to_string(category.current_id + 1) + ", ";
        sqlInsert += std::to_string(color.current_id + 1) + ", ";
        sqlInsert += "'" + quality.desc + "', "; // Assuming quality is TEXT
        sqlInsert += "1)"; // Assuming quantity is initialized to 0
        int rc = sqlite3_exec(partsStock.db, sqlInsert.c_str(), 0, 0, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(partsStock.db));
        }
        if (rc == SQLITE_DONE) {
            // Update successful
        }
        std::cout << "Part added to stock." << std::endl;

        sqlite3_close(partsStock.db);

    }
}

//std::set<std::string> TokenizeAndStore(const std::string& input) {
//    std::set<std::string> words;
//    std::istringstream iss(input);
//    std::string word;
//
//    while (iss >> word) {
//        words.insert(word);
//    }
//    return words;
//}

//int PartsStockWindow::SearchForSimilarRecords() {
//    partsStock.OpenPartsStockDb();
//    std::set<std::string> words1 = TokenizeAndStore(quality.desc);
//    const char* sqlQuery = "SELECT part_id, quality FROM parts WHERE brand_id = ? AND model_id = ? AND category_id = ? AND color = ?";
//    sqlite3_stmt* stmt;
//    if (sqlite3_prepare_v2(partsStock.db, sqlQuery, -1, &stmt, NULL) == SQLITE_OK) {
//        // Bind the parameter values to the placeholders in the query
//        int brandId = brand.current_id + 1;
//        int modelId = model.current_id + 1;
//        int categoryId = category.current_id + 1;
//        int colorId = color.current_id + 1;
//
//        sqlite3_bind_int(stmt, 1, brandId);
//        sqlite3_bind_int(stmt, 2, modelId);
//        sqlite3_bind_int(stmt, 3, categoryId);
//        sqlite3_bind_int(stmt, 4, colorId);
//
//        // Execute the query
//        while (sqlite3_step(stmt) == SQLITE_ROW) {
//            // Retrieve data from the result set
//            int partId = sqlite3_column_int(stmt, 0);
//            const char* quality = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
//            std::set<std::string> words2 = TokenizeAndStore(quality);
//            if (words1 == words2) {
//                std::cout << "TRUE FOR: " << quality <<  " " << partId << std::endl;
//            
//                sqlite3_finalize(stmt);
//                sqlite3_close(partsStock.db);
//                return partId;
//            }
//            std::cout << " Quality: " << quality << std::endl;
//        }
//
//        // Finalize the statement
//        sqlite3_finalize(stmt);
//        sqlite3_close(partsStock.db);
//        return 0;
//    }
//    else {
//        // Handle the error
//        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(partsStock.db) << std::endl;
//    }
//    sqlite3_finalize(stmt);
//    sqlite3_close(partsStock.db);
//    return 0;
//
//}



void PartsStockWindow::GetBrands() {
    if (!brand.retreived) {
        const char* brandsQuery = "SELECT brand FROM brands";
        sqlQuery.AllFromTable(brandsQuery, brand.data);
        sqlite3_close(partsStock.db);
        brand.retreived = true;
    }
}

void PartsStockWindow::GetModels() {
    if (!model.retreived) {
        const char* currentBrand = brand.data[brand.current_id].c_str();
        int brand_id = sqlQuery.GetIdForValue("brands", "brand", currentBrand);
        const char* modelsQuery = "SELECT model FROM models WHERE brand_id = ?";
        sqlQuery.OnID(modelsQuery, model.data, brand_id);
        model.retreived = true;
    }
}

void PartsStockWindow::GetCategories() {
    if (!category.retreived) {
        const char* categoriesQuery = "SELECT category FROM categories";
        sqlQuery.AllFromTable(categoriesQuery, category.data);
        category.retreived = true;
    }
}

void PartsStockWindow::GetQualities() {
    if (!quality.retreived) {
        const char* qualitiesQuery = "SELECT quality FROM qualities";
        sqlQuery.AllFromTable(qualitiesQuery, quality.data);
        quality.retreived = true;
    }

}

void PartsStockWindow::GetColorsForModel() {
    if (!color.retreived) {
        const char* currentModel = model.data[model.current_id].c_str();
        int model_id = sqlQuery.GetIdForValue("models", "model", currentModel);
        const char* colorsQuery = "SELECT color FROM colors WHERE color_id IN (SELECT color_id FROM model_color WHERE model_id = ?)";
        sqlQuery.OnID(colorsQuery, color.data, model_id);
        color.retreived = true;
    }
}

void PartsStockWindow::ResetOnBrandChange() {
    if (brand.current_id != previous_brand_id) {
        std::cout << "RESET" << std::endl;
        previous_brand_id = brand.current_id;

        category.current_id = -1;
        category.retreived = false;

        color.current_id = -1;
        color.retreived = false;

        model.current_id = -1;
        model.data.clear();
        model.retreived = false;

        category.data.clear();
        color.data.clear();

        quality.order.clear();
        quality.desc = "";
        quality.retreived = false;
        quality.data.clear();
        for (bool& value : quality.selections) { value = false; }
    }
}
