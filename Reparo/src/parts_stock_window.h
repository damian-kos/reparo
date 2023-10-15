#pragma once

#include "imgui.h"
#include <iostream>
#include "parts_stock.h"
#include "sql_queries.h"
#include "imgui_helper.h"
#include "structs.h"

void ClearPart(PartData& part);

class PartsStockWindow {
public:
	PartsStock partsStock;
	SQLQuery sqlQuery;
	ImGuiHelper imguiHelper;
	Part part;
	void Render();
	void ResetOnBrandChange();
	void ResetOnModelChange(PartData& model, PartData& color, int& previous_id);
	void GetStates(PartData& data);
	void AddPart();
	void GetBrands();
	void GetModels(std::vector<std::string>& data, int selected_brand_id);

	void GetCategories(std::vector<std::string>& data);
	void GetQualities();
	void GetColorsForModel(std::vector<std::string>& data, std::vector<std::string>& model_data, std::string selected_model_id);
	void GetColorsForModel(std::vector<std::string>& data, std::vector<std::string>& model_data, int selected_model_id);
};