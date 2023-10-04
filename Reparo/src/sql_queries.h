#pragma once
#include <sqlite3.h>
#include <vector>
#include <string>
#include "parts_stock.h"

class SQLQuery {
public:
    PartsStock partsStock;
    void QueryAllFromTable(const char* query, std::vector<std::string>& vector);;
};