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

public:
    PartsStock partsStock;
    void AllFromTable(const char* query, std::vector<std::string>& vector); // Takes all names from table
    void OnID(const char* query, std::vector<std::string>& vector, int id); // Takes all names from table WHERE id = ?
    int GetIdForValue(std::string query, const char* searchValue); // Returns if for a given value
    int SearchForSimilarRecords(Part part); // Check if quality of part we are about to add already exists
    void Update(int& rowToUpdate); // Updates the quantity of the exactly same part by 1
    void InsertPart(Part& part); // Insert part if one does not exist w/ quantity of 1
    int SearchForCustomerSQL(Customer customer); // Seearch for a customer with exact given number
    int InsertCustomer(Customer customer); // Insert new customer and returns ID it was inserted with
    void UpdateCustomer(int& rowToUpdate, Customer customerEdit); // Upadtes customer data wtih given ID
    void MatchingCustomers(std::string& partial_phone_number, std::unordered_map<int, Customer>& customers); // Search for customers according to name/email/phone/surname
    void MatchingModels(std::string& model, std::vector<std::string>& vector, std::string label);  
    void AddRepair(Repair& repair, int customerID);
    std::vector<Repair> GetAllToDoRepairs();
    void Prices();
};