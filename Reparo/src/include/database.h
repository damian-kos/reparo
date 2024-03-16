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

struct RepairUpdates {
  std::string date;
  std::string note;
};

struct RepairsSort {
  std::unordered_map<int, Repair> repairs;
  std::vector<int> repairs_order;
  double total = 0.0f;
};

struct DevicesSort {
  std::unordered_map<int, DeviceDetailed> devices;
  std::vector<int> devices_order;
};


class Database {
public:
    Database();
    ~Database();
public:
    static int QueryCustomerIDByPhone(std::string phone);
    static Customer* QueryCustomerByPhone(std::string phone);
    static void InsertCustomer(Customer& customer, int* last_row_id);
    static void ManageSearchState(const char* label, Attribute& attribute, const char* buffer);
    static void ManageSearchState(const char* label, Attribute& attribute, int rel_id, const char* buffer);
    static int GetIDForValueS(const char* value, const char* label);
    static bool GetBoolForValue(const char* label, const char* buffer);
    static void InsertRepair(Repair& repair);
    static int InsertCustomDevice(Repair& repair);
    static std::unordered_map<int, std::string> GetSimpleTable(const std::string& table);
    static void UpdateCustomer(Customer& customer, int id);
    static void UpdateRepair(Repair& repair, int id);
    static int GetIDForID(int id, std::string table); 
    static void InsertRepairUpdateDesc(int& repair_id, std::string& desc);
    static std::vector<RepairUpdates> RetreiveRepairUdpdates(int& repair_id);
    static std::vector<std::string> GetSimpleTableNames();
    static void DeleteRepair(int& repair_id);
    static RepairsSort RetreiveRepairsByDate(std::string* date, 
                                             int date_direction = 0, 
                                             std::string* date_2 = nullptr, 
                                             int state = 0, int asc_desc = 1,
                                             int order_by = 10);
    static DevicesSort RetreiveDevices(const int type = 0, const int brand = 0, const std::string search_query = "", int* asc_desc = nullptr);
    static DeviceDetailed RetreiveDevice(const int& id);
    static void UpdateDeviceDetails(DeviceDetailed& device, const std::string& attr, const std::string& value);
    static void DeleteDeviceDetail(DeviceDetailed& device, const std::string& attr, const std::string& value);
    //DevicesSort RetreiveDevices();
    static sqlite3* PtrDB();

private:
    sqlite3* db = nullptr;

private:
    void OpenDB();
};