#pragma once

#include "imgui.h"
#include <sqlite3.h>
#include "parts_stock.h"
#include "sql_queries.h"


void PopulateListBox(const char* label, std::vector<std::string>& vector, int& selectable);
void PopulateListBoxMulti(const char* label, std::vector<std::string>& vector, bool(&selectables)[20]);
void ResetOnBrandChange();

struct DataPair {
	int current_id = -1;
	std::vector<std::string> data;
	bool retreived;
};

class PartsStockWindow {
public:
	PartsStock partsStock;
	SQLQuery sqlQuery;
	DataPair brand;
	DataPair model;
	DataPair category;
	DataPair color;
	void ResetOnBrandChange();

	void Render();
	void AddPart();
	int SearchForSimilarRecords();
	void GetBrands();
	void GetModels();
	void GetCategories();
	void GetQualities();
	void GetColorsForModel();
	int GetIdForValue(const char* tableName, const char* columnName, const char* searchValue);
	void Query(const char* query, std::vector<std::string>& vector);
	void QueryRelationalTables(const char* query, std::vector<std::string>& vector, int id);


};