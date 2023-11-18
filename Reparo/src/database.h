#pragma once
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "structs.h"
#include "enums.h"

class Database {
public:
    Database();
    ~Database();
public:
    int QueryCustomerByPhone(std::string phone);

    void InsertCustomer(Customer& customer, int* last_row_id);



    void ManageSearchState(const char* label, Attribute& attribute, const char* buffer);

    void ManageSearchState(const char* label, Attribute& attribute, const char* buffer, int rel_id);

    int GetIDForValue(const char* label, const char* value);

    bool GetBoolForValue(const char* label, const char* buffer);
    void InsertRepair(Repair repair);

    std::map<int, Repair> RetreiveRepairsOfState(int state);

private:
    sqlite3* db = nullptr;

private:
    void OpenDB();
};