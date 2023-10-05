#pragma once
#include <sqlite3.h>
#include "parts_stock.h"
#include "input_field.h"
#include <set>
#include <sstream>

class SQLQuery {
public:
    PartsStock partsStock;
    void AllFromTable(const char* query, std::vector<std::string>& vector);
    void OnID(const char* query, std::vector<std::string>& vector, int id);
    int GetIdForValue(const char* tableName, const char* columnName, const char* searchValue);
    int SearchForSimilarRecords(Part part);
    void Update(int& rowToUpdate);
    void InsertPart(Part part);
};