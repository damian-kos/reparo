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
	void ResetOnModelChange();

	void AddPart();
	void GetBrands();
	void GetModels();
	void GetCategories();
	void GetQualities();
	void GetColorsForModel();

};