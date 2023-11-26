#pragma once
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <time.h>

#include "structs.h"
#include "enums.h"

class Database {
public:
    Database();
    ~Database();
public:
    int QueryCustomerIDByPhone(std::string phone);
    Customer* QueryCustomerByPhone(std::string phone);
    void InsertCustomer(Customer& customer, int* last_row_id);
    
    static void ManageSearchState(const char* label, Attribute& attribute, const char* buffer);
    static void ManageSearchState(const char* label, Attribute& attribute, int rel_id, const char* buffer);
    static int GetIDForValueS(const char* value, const char* label);

    bool GetBoolForValue(const char* label, const char* buffer);
    void InsertRepair(Repair repair);
    std::map<int, Repair> RetreiveRepairsOfState(int state);
    std::map<int, std::string> GetRepairStates();
    void UpdateCustomer(Customer& customer, int id);
    void UpdateRepair(Repair& repair, int id);
    int GetIDForID(int id, std::string table);
    static sqlite3* PtrDB();

private:
    sqlite3* db = nullptr;
    tm date = {};

private:
    void OpenDB();
};