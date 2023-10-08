#pragma once
#include <sqlite3.h>
#include "parts_stock.h"
#include "structs.h"
#include <set>
#include <sstream>
#include <unordered_map>


class SQLQuery {
public:
    PartsStock partsStock;
    void AllFromTable(const char* query, std::vector<std::string>& vector);
    void OnID(const char* query, std::vector<std::string>& vector, int id);
    int GetIdForValue(const char* tableName, const char* columnName, const char* searchValue);
    int SearchForSimilarRecords(Part part);
    void Update(int& rowToUpdate);
    void UpdateCustomer(int& rowToUpdate, Customer customerEdit);
    void InsertPart(Part part);
    void InsertCustomer(Customer customer);
    int SearchForCustomerSQL(Customer customer);
    void MatchingCustomers(std::string& partial_phone_number, std::unordered_map<int, Customer>& customers);
};