#pragma once
#include <sqlite3.h>
#include "parts_stock.h"
#include "structs.h"
#include <set>
#include <sstream>
#include <unordered_map>

std::set<std::string> TokenizeAndStore(const std::string& input);

class SQLQuery {
public:
    PartsStock partsStock;
    void AllFromTable(const char* query, std::vector<std::string>& vector); // Takes all names from table
    void OnID(const char* query, std::vector<std::string>& vector, int id); // Takes all names from table WHERE id = ?
    int GetIdForValue(std::string query, const char* searchValue); // Returns if for a given value
    int SearchForSimilarRecords(Part part); // Check if part we are about to add is alread
    void Update(int& rowToUpdate);
    void UpdateCustomer(int& rowToUpdate, Customer customerEdit);
    void InsertPart(Part& part);
    int InsertCustomer(Customer customer);
    int SearchForCustomerSQL(Customer customer);
    void MatchingCustomers(std::string& partial_phone_number, std::unordered_map<int, Customer>& customers);
    void MatchingModels(std::string& model, std::vector<std::string>& vector, std::string label);
    void AddRepair(Repair& repair, int customerID);
    void Prices();
};