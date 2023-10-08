//#include "structs.h
#include "parts_stock_window.h"

char partQuery[128] = "";

static int previous_brand_id = -1;
static int previous_model_id = -1;


void PartsStockWindow::Render() {
  
    ImGui::Begin("Add part to stock");
    ImGui::InputTextWithHint(
        "##SearchPart", "Part to search... ", partQuery, 
        IM_ARRAYSIZE(partQuery), ImGuiInputTextFlags_ReadOnly);

    imguiHelper.PartTableStockWindow(part);

    ImGui::PushItemWidth(128);
    imguiHelper.PopulateListBox("##Brand", part.brand.data, part.brand.current_id, part.brand.name);

    ResetOnBrandChange();

    if (part.brand.current_id != -1) {
        GetModels();
    }

    imguiHelper.PopulateListBox("##Models", part.model.data, part.model.current_id, part.model.name);
    ResetOnModelChange();
    
    if (part.model.current_id != -1) {
        GetCategories();
        if (part.category.current_id+1 == 1 || part.category.current_id + 1 == 4)  {

            GetColorsForModel();
        }
        else {
            part.color.current_id = -1;
            part.color.data.clear();
            part.color.retreived = false;
        }
    }

    imguiHelper.PopulateListBox("##Categories", part.category.data, part.category.current_id, part.category.name);
    imguiHelper.PopulateListBox("##Colors", part.color.data, part.color.current_id, part.color.name);

    if (part.category.current_id != -1) {
        GetQualities();
        imguiHelper.PopulateListBoxMulti("##Qualities", part.quality);
    }
    if (ImGui::Button("Add part to stock..")) {
        AddPart();
    }
    ImGui::End();
}


void PartsStockWindow::AddPart()
{
    if (part.brand.current_id == -1 || part.model.current_id == -1 || part.category.current_id == -1) { return; }
    int rowToUpdate = sqlQuery.SearchForSimilarRecords(part);
    std::cout << "ROW TO UPDATE: " << rowToUpdate << std::endl;
    if (rowToUpdate > 0) {
        sqlQuery.Update(rowToUpdate);
    }
    else {
        sqlQuery.InsertPart(part);
    }
}

void PartsStockWindow::GetBrands() {
    if (!part.brand.retreived) {
        const char* brandsQuery = "SELECT brand FROM brands";
        sqlQuery.AllFromTable(brandsQuery, part.brand.data);
        sqlite3_close(partsStock.db);
        part.brand.retreived = true;
    }
}

void PartsStockWindow::GetModels() {
    if (!part.model.retreived) {
        const char* currentBrand = part.brand.data[part.brand.current_id].c_str();
        int brand_id = sqlQuery.GetIdForValue("brands", "brand", currentBrand);
        const char* modelsQuery = "SELECT model FROM models WHERE brand_id = ?";
        sqlQuery.OnID(modelsQuery, part.model.data, brand_id);
        part.model.retreived = true;
    }
}

void PartsStockWindow::GetCategories() {
    if (!part.category.retreived) {
        const char* categoriesQuery = "SELECT category FROM categories";
        sqlQuery.AllFromTable(categoriesQuery, part.category.data);
        part.category.retreived = true;
    }
}

void PartsStockWindow::GetQualities() {
    if (!part.quality.retreived) {
        const char* qualitiesQuery = "SELECT quality FROM qualities";
        sqlQuery.AllFromTable(qualitiesQuery, part.quality.data);
        part.quality.retreived = true;
    }
}

void PartsStockWindow::GetColorsForModel() {
    if (!part.color.retreived) {
        const char* currentModel = part.model.data[part.model.current_id].c_str();
        int model_id = sqlQuery.GetIdForValue("models", "model", currentModel);
        const char* colorsQuery = "SELECT color FROM colors WHERE color_id IN (SELECT color_id FROM model_color WHERE model_id = ?)";
        sqlQuery.OnID(colorsQuery, part.color.data, model_id);
        part.color.retreived = true;
    }
}

void ClearPart(PartData& part) {
    part.current_id = -1;
    part.data.clear();
    part.name = "";
    part.retreived = false;
}

void PartsStockWindow::ResetOnModelChange() {
    if (part.model.current_id != previous_model_id) {
        std::cout << "RESET" << std::endl;
        ClearPart(part.color);
        GetColorsForModel();
        previous_model_id = part.model.current_id;
    }
}

void PartsStockWindow::ResetOnBrandChange() {
    if (part.brand.current_id != previous_brand_id) {
        std::cout << "RESET" << std::endl;
        previous_brand_id = part.brand.current_id;

        ClearPart(part.model);
        ClearPart(part.category);
        ClearPart(part.color);
        part.quality.order.clear();
        part.quality.desc = "";
        part.quality.retreived = false;
        part.quality.data.clear();
        for (bool& value : part.quality.selections) { value = false; }
    }
}

