#pragma once

#include "imgui.h"
#include <sqlite3.h>
#include "parts_stock.h"
#include "sql_queries.h"
#include "imgui_helper.h"
#include "input_field.h"



class PartsStockWindow {
public:
	PartsStock partsStock;
	SQLQuery sqlQuery;
	ImGuiHelper imguiHelper;
	//PartData brand;
	//PartData model;
	//PartData category;
	//PartData color;
	//PartQualityData quality;
	Part part;
	void Render();
	void ResetOnBrandChange();
	void AddPart();
	//int SearchForSimilarRecords();
	void GetBrands();
	void GetModels();
	void GetCategories();
	void GetQualities();
	void GetColorsForModel();

};