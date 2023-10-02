#pragma once

#include "imgui.h"
#include <sqlite3.h>
#include "parts_stock.h"

void PopulateListBox(const char* label, std::vector<std::string>& vector, int& selectable);
void PopulateListBoxMulti(const char* label, std::vector<std::string>& vector, bool(&selectables)[20]);
void ResetOnBrandChange();
class PartsStockWindow {
public:
	void Render();
	void GetBrands();
	void GetModels();
	void GetCategories();
	void GetQualities();
	void GetColorsForModel();
	int GetIdForValue(const char* tableName, const char* columnName, const char* searchValue);
	void Query(const char* query, std::vector<std::string>& vector);
	void QueryRelationalTables(const char* query, std::vector<std::string>& vector, int id);
	PartsStock partsStock;


};