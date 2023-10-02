#pragma once

#include "imgui.h"
#include <sqlite3.h>

class PartsStock {
public:
	sqlite3* db;
	void OpenPartsStockDb();
	void CreateTable();
	void PrintOutData();
};