#include "database.h"
#include <sstream>
Database::Database() {
    printf("Database created\n");
}

Database::~Database() {
    std::cout << "Database destroyed" << std::endl;
}

//void Database::OpenDB() {
//    db = nullptr;
//    int rc = sqlite3_open("../parts_stock.db", &db);
//    if (rc != SQLITE_OK) {
//        std::cout << "DATABASE CAN'T BE OPEN" << std::endl;
//    }
//    std::cout << "DB IS OPEN" << std::endl;
//}

sqlite3* Database::PtrDB() {
    sqlite3* db_ptr = nullptr;
    int rc = sqlite3_open("vendor/parts_stock.db", &db_ptr);
    if (rc != SQLITE_OK) {
        std::cout << "DATABASE CAN'T BE OPEN" << std::endl;
        return db_ptr;
    }
    //std::cout << "DB IS OPEN" << std::endl;
    return db_ptr;
}

int Database::QueryCustomerIDByPhone(std::string phone) {
    std::cout << "QueryCustomerIDByPhone is running  " << std::endl;
    int result = -1;
    sqlite3* db_ptr = PtrDB();
    if (phone.empty()) { result = -2; }
    const char* query = "SELECT customer_id FROM customers WHERE phone = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_ptr, query, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, phone.c_str(), static_cast<int>(phone.size()), NULL); // static cast to avoid getting arg conversion warning
        
        int customerID = 0;
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            customerID = sqlite3_column_int(stmt, 0);
        }
        result = (customerID !=0) ? customerID : 0; // For new customer
    }
    else {
        result =  -1; // Error querying db
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db_ptr);
    return result;
}

Customer* Database::QueryCustomerByPhone(std::string phone) {
    Customer* customer;
    std::string name;
    std::string surname;
    std::string email;
    std::cout << "QueryCustomerByPhone is running  w/ PHONE:" << phone << std::endl;
    sqlite3* db_ptr = PtrDB();
    const char* query = "SELECT * FROM customers WHERE phone = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_ptr, query, -1, &stmt, NULL) == SQLITE_OK) {
      sqlite3_bind_text(stmt, 1, phone.c_str(), static_cast<int>(phone.size()), NULL); // static cast to avoid getting arg conversion warning
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << "Statement step " << std::endl;
            name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            customer = new Customer(phone, name, surname, email);
            std::cout << customer->name << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db_ptr);
            return customer;
        }
        //customer = customer->name.empty() ? nullptr : customer;
        //customer = Customer(phone, name, surname, email);
        sqlite3_finalize(stmt);
        sqlite3_close(db_ptr);
        return nullptr;
    }
    else {
        std::cerr << "Error preparing SQL statement QueryCustomerByPhone: " << sqlite3_errmsg(db_ptr) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db_ptr);
        return nullptr; // Error querying db_ptr
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db_ptr);
    return customer;
}

void Database::InsertCustomer(Customer& customer, int* last_row_id) {
    std::cout << "InsertCustomer is running  " << std::endl;
    sqlite3* db_ptr = PtrDB();
    const char* query = "INSERT INTO customers (name, surname, email, phone) VALUES (?, ?, ?, ?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_ptr, query, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, customer.name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, customer.surname.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, customer.email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, customer.phone.c_str(), -1, SQLITE_STATIC);
    }

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (last_row_id != nullptr) {
          *last_row_id = static_cast<int>(sqlite3_last_insert_rowid(db_ptr));
        }
        std::cout << "Customer added: " << customer.phone << " " << customer.name << std::endl;
    }
    else {
        std::cout << "Error during adding a customer" << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db_ptr);
}

void Database::ManageSearchState(const char* label, Attribute& attribute, const char* buffer) {
    std::cout << "ManageSearchState is running  w/ buffer: " << buffer << std::endl;
    
    sqlite3* db_ptr = PtrDB();
    if (db_ptr == nullptr)
        return;
    //OpenDB();
    static std::map<const char*, const char*> queries = {
    {"##Model", "SELECT model FROM models WHERE model LIKE ?"},
    {"##Category", "SELECT category FROM categories WHERE category LIKE ?"},
    {"##Phone", "SELECT phone, name, surname FROM customers WHERE phone = ?"},
    {"##PartialPhone", "SELECT phone, name, surname FROM customers WHERE phone LIKE ?"},

    };
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_ptr, queries[label], -1, &stmt, NULL) == SQLITE_OK) {
        std::string buffer_str = buffer;
        std::string like_query = (label != "##Phone") ? "%" + buffer_str + "%" : buffer_str;
        sqlite3_bind_text(stmt, 1, like_query.c_str(), -1, SQLITE_STATIC);
        int column_count = sqlite3_column_count(stmt);
        
        attribute.data.clear();
        int record_count = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (record_count == 5) {
                break;
            }
            std::ostringstream concatenatedValue;

            for (int i = 0; i < column_count; ++i) {
                const char* column_value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));

                // Append the value to the concatenated string
                concatenatedValue << column_value << ' ';
            }

            // Get the concatenated string without the trailing space
            std::string value = concatenatedValue.str();
            //std::cout << "concatenatedValue: " << value << std::endl;
            if (!value.empty()) {
                value.pop_back();
            }

            attribute.data.emplace_back(value);

            record_count++;
        }
    }
    else {
        std::cerr << "Error preparing SQL statement ManageSearchState (3 param): " << sqlite3_errmsg(db_ptr) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db_ptr);

}

void Database::ManageSearchState(const char* label, Attribute& attribute, int rel_id, const char* buffer) {
    std::cout << "ManageSearchState 2 is running  STATIC  with ID: " << rel_id << std::endl;
    sqlite3* db_ptr = PtrDB(); 
    static std::map<const char*, const char*> queries = {
    {"##Color", "SELECT color FROM colors WHERE color_id IN (SELECT color_id FROM model_color WHERE model_id = ?)"}, 
    };
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_ptr, queries[label], -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, rel_id);
        attribute.data.clear();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            attribute.data.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        }
    }
    else {
        std::cerr << "Error preparing SQL statement ManageSearchState (4 param): " << sqlite3_errmsg(db_ptr) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db_ptr);
}

int Database::GetIDForValueS(const char* label, const char* value) {
    std::cout << "GetIDForValue is running S with value:|" << value << "|" << std::endl;

    sqlite3* db_ptr = PtrDB();
    static std::map<const char*, const char*> queries = {
    {"##Customer", "SELECT customer_id FROM customers WHERE LOWER(phone) = LOWER(?)"},
    {"##Model", "SELECT model_id FROM models WHERE LOWER(model) = LOWER(?)"},
    {"##Category", "SELECT category_id FROM categories WHERE LOWER(category) = LOWER(?)"},
    {"##Color", "SELECT model_id FROM models WHERE LOWER(model) = LOWER(?)"},
    {"##Color2", "SELECT color_id FROM colors WHERE LOWER(color) = LOWER(?)"},
    {"##State", "SELECT repair_state_id FROM repair_states WHERE LOWER(repair_state) = LOWER(?)"},
    };
    int id = -1;
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_ptr, queries[label], -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, value, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            id = sqlite3_column_int(stmt, 0);
        }
        else {
            std::cerr << "GetIDForValue: No rows returned." << std::endl;
        }
    }
    else {
        std::cerr << "Error preparing SQL statement: GetIDForValueS   " << sqlite3_errmsg(db_ptr) << std::endl;
        id = -1; // error
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db_ptr);

    return id;
}

bool Database::GetBoolForValue(const char* label, const char* buffer) {
    std::cout << "GetBoolForValue is running  " << std::endl;
    sqlite3* db_ptr = PtrDB();
    static std::map<const char*, const char*> queries = {
    {"##Model", "SELECT model_id FROM models WHERE LOWER(model) = LOWER(?)"},
    {"##Category", "SELECT category_id FROM categories WHERE LOWER(category) = LOWER(?)"},
    {"##Color", "SELECT color_id FROM colors WHERE LOWER(color) = LOWER(?)"},
    };
    bool is_found = false;
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_ptr, queries[label], -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, buffer, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
 
            is_found = true;
        }
        else {
            std::cerr << "GetBoolForValue: No rows returned. " << std::endl;
        }
    }
    else {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db_ptr) << std::endl;
        is_found = false; // error
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db_ptr);

    return is_found;
}

void Database::InsertRepair(Repair& repair) {
    std::cout << "InsertRepair is running: " <<  &repair <<std::endl;
    int customer_id = GetIDForValueS("##Customer", repair.customer.phone.c_str());
    int model_id = GetIDForValueS("##Model", repair.device.name.c_str());
    int cust_device_id = -1;
    if (model_id < 0) {
      cust_device_id = InsertCustomDevice(repair);
    }
    int category_id = GetIDForValueS("##Category", repair.category.c_str());
    int color_id = GetIDForValueS("##Color2", repair.device.color.c_str());
    std::cout << "INSERT REPAIR: " << customer_id << " | " << model_id << " | " << category_id << " | " << color_id << std::endl;
    sqlite3* db_ptr = PtrDB();
    sqlite3_stmt* stmt; 
    const char* query = "INSERT INTO repairs"
        "(customer_id, model_id, category_id, color_id, repair_desc, repair_desc_hidden, price, repair_state_id, date, sn_imei, cust_device_id) "
        "VALUES(?, ?, ?, ?, ?, ?, ?, ?, datetime('now', 'localtime'), ?, ?)";
    if (sqlite3_prepare_v2(db_ptr, query, -1, &stmt, NULL) == SQLITE_OK) {

        sqlite3_bind_int(stmt, 1, customer_id);
        sqlite3_bind_int(stmt, 2, model_id);
        sqlite3_bind_int(stmt, 3, category_id);
        sqlite3_bind_int(stmt, 4, color_id);
        sqlite3_bind_text(stmt, 5, repair.visible_note.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, repair.hidden_note.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 7, repair.price);
        sqlite3_bind_int(stmt, 8, static_cast<int>(RepairState::TO_DO)); // repair_state_id
        sqlite3_bind_text(stmt, 9, repair.sn_imei.c_str(), -1, SQLITE_STATIC); // sn_imei
        if (cust_device_id > 0) {
          sqlite3_bind_int(stmt, 10, cust_device_id);
        }

    }
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Repair added: " << repair.device.name << " " << repair.customer.phone << std::endl;
        //if (last_row_id != nullptr) {
        repair.id = static_cast<int>(sqlite3_last_insert_rowid(db_ptr));
        //}
    }
    else {
        std::cout << "Error during adding a repair: " << sqlite3_errmsg(db_ptr) << " " << sqlite3_errcode(db_ptr) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db_ptr);
}

int Database::InsertCustomDevice(Repair& repair) {
  std::cout << "InsertCustomDevice is running  " << std::endl;
  int customer_id = GetIDForValueS("##Customer", repair.customer.phone.c_str());
  int category_id = GetIDForValueS("##Category", repair.category.c_str());
  const char* query = "INSERT INTO custom_device"
    "(brand, model, color) VALUES(?, ?, ?)";

  static int last_row_id;

  sqlite3* db_ptr = PtrDB();
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db_ptr, query, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, "" , -1, SQLITE_STATIC); // brand
    sqlite3_bind_text(stmt, 2, repair.device.name.c_str(), -1, SQLITE_STATIC); // model
    sqlite3_bind_text(stmt, 3, repair.device.color.c_str(), -1, SQLITE_STATIC); // color

  }
  if (sqlite3_step(stmt) == SQLITE_DONE) {
    std::cout << "Customdevice added: " << repair.device.name << " " << repair.customer.phone << std::endl;
    last_row_id = static_cast<int>(sqlite3_last_insert_rowid(db_ptr));
  }
  else {
    std::cout << "Error during adding a repair: " << sqlite3_errmsg(db_ptr) << " " << sqlite3_errcode(db_ptr) << std::endl;
    last_row_id = -1;
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db_ptr);
  return last_row_id;
}

std::unordered_map<int, std::string> Database::GetRepairStates() {
    std::cout << "GetRepairsStates is running " << std::endl;
    sqlite3* db_ptr = PtrDB();
    static std::unordered_map<int, std::string> states;
    sqlite3_stmt* stmt;
    const char* query = "SELECT * FROM repair_states";
    if (sqlite3_prepare_v2(db_ptr, query, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int state_id = sqlite3_column_int(stmt, 0);
            std::string state = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            states.insert(std::make_pair(state_id, state));
        }
    }
    else {
        std::cout << "States couldn't be retreived " << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db_ptr);
    return states;
}

void Database::UpdateCustomer(Customer& customer, int id) {
    std::cout << "UpdateCustomer is running" << std::endl;
    sqlite3* db_ptr = PtrDB();
    sqlite3_stmt* stmt;
    const char* query = "UPDATE customers SET name = ?, surname = ?, email = ?, phone = ? WHERE customer_id = ?";
    if (sqlite3_prepare_v2(db_ptr, query, -1, &stmt, NULL) == SQLITE_OK) {
        std::cout << "Update will happen" << std::endl;
        std::cout << id << " / " << customer.name.c_str() << " / " << customer.surname.c_str() << " /  " << customer.email.c_str() << " / " << customer.phone.c_str() << std::endl;
        sqlite3_bind_text(stmt, 1, customer.name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, customer.surname.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, customer.email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, customer.phone.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error updating customer: " << sqlite3_errmsg(db_ptr) << std::endl;
        }
    }
    else {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db_ptr) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db_ptr);
}

void Database::UpdateRepair(Repair& repair, int id) {
  std::cout << "UpdateRepair is running" << std::endl;
  static int model_id = GetIDForValueS("##Model", repair.device.name.c_str());
  static int category_id = GetIDForValueS("##Category", repair.category.c_str());
  static int color_id = GetIDForValueS("##Color2", repair.device.color.c_str());
  sqlite3* db_ptr = PtrDB();
  sqlite3_stmt* stmt;
  const char* query = "UPDATE repairs SET model_id = ?, category_id = ?, color_id = ?, repair_desc = ?, repair_desc_hidden = ?, price = ?, repair_state_id = ?, sn_imei = ? WHERE repair_id = ?";
  if (sqlite3_prepare_v2(db_ptr, query, -1, &stmt, NULL) == SQLITE_OK) {
    std::cout << "Update Repairwill happen" << std::endl;
    //std::cout << id << " / " << customer.name.c_str() << " / " << customer.surname.c_str() << " /  " << customer.email.c_str() << " / " << customer.phone.c_str() << std::endl;
    sqlite3_bind_int(stmt, 1, model_id);
    sqlite3_bind_int(stmt, 2, category_id);
    sqlite3_bind_int(stmt, 3, color_id);
    sqlite3_bind_text(stmt, 4, repair.visible_note.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, repair.hidden_note.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 6, repair.price);
    std::cout << "Repair State: |" << repair.state << "|" << std::endl;
    int state_id = GetIDForValueS("##State", repair.state.c_str());
    sqlite3_bind_int(stmt, 7, state_id);
    sqlite3_bind_text(stmt, 8, repair.sn_imei.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 9, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error updating repair: " << sqlite3_errmsg(db_ptr) << std::endl;
    }
  }
  else {
    std::cerr << "Error preparing UpdateRepair statement: " << sqlite3_errmsg(db_ptr) << std::endl;
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db_ptr);
}

int Database::GetIDForID(int id, std::string table) {
    int found_id = -1;
    std::cout << "GetIDForValue is running " << std::endl;
    static std::unordered_map<std::string, const char*> queries = {
    {"repairs", "SELECT customer_id FROM repairs WHERE repair_id = ?"},
    };
    sqlite3* db_ptr = PtrDB();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_ptr, queries[table], -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            found_id = sqlite3_column_int(stmt, 0);
        }
        else {
            std::cerr << "GetIDForID: No rows returned." << std::endl;
        }
    }
    else {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db_ptr) << std::endl;
        found_id = -1; // error
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db_ptr);

    return found_id;
}


void Database::InsertRepairUpdateDesc(int& repair_id, std::string& desc) {
  sqlite3* db_ptr = PtrDB();
  sqlite3_stmt* stmt;
  const char* query = "INSERT INTO repairs_update (repair_id, update_description, date) VALUES (?, ?, datetime('now', 'localtime'))";
  if (sqlite3_prepare_v2(db_ptr, query, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, repair_id);
    sqlite3_bind_text(stmt, 2, desc.c_str(), -1, SQLITE_STATIC);

  }
  if (sqlite3_step(stmt) == SQLITE_DONE) {
    std::cout << "Update added: " << desc << std::endl;
  }
  else {
    std::cout << "Error during adding an update information: " << sqlite3_errmsg(db_ptr) << " " << sqlite3_errcode(db_ptr) << std::endl;
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db_ptr);
}

std::vector<RepairUpdates> Database::RetreiveRepairUdpdates(int& repair_id) {
  sqlite3* db_ptr = PtrDB();
  sqlite3_stmt* stmt;
  std::vector<RepairUpdates> updates;
  const char* query = "SELECT * FROM repairs_update WHERE repair_id = ? ORDER BY date DESC";
  if (sqlite3_prepare_v2(db_ptr, query, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, repair_id);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      RepairUpdates update;
      update.note = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
      update.date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
      updates.emplace_back(update);
    }
  }
  else {
    std::cout << "Error during retreiving an update information: " << sqlite3_errmsg(db_ptr) << " " << sqlite3_errcode(db_ptr) << std::endl;
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db_ptr);
  return updates;
}

std::vector<std::string> Database::GetRepairStatesNames() {
  sqlite3* db_ptr = PtrDB();
  sqlite3_stmt* stmt;
  std::vector<std::string> states;
  const char* query = "SELECT repair_state FROM repair_states ORDER BY repair_state_id ASC";
  if (sqlite3_prepare_v2(db_ptr, query, -1, &stmt, NULL) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      std::string state = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
      states.emplace_back(state);
    }
  }
  else {
    std::cout << "Error during retreiving an update information: " << sqlite3_errmsg(db_ptr) << " " << sqlite3_errcode(db_ptr) << std::endl;
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db_ptr);
  return states;
}

void Database::DeleteRepair(int& repair_id) {
  printf("DeleteRepair is running\n");
  sqlite3* db_ptr = PtrDB();
  sqlite3_stmt* stmt;
  std::vector<std::string> states;
  const char* query = "DELETE FROM repairs WHERE repair_id  = ?";
  if (sqlite3_prepare_v2(db_ptr, query, -1, &stmt, NULL) == SQLITE_OK) {
    sqlite3_bind_int(stmt, 1, repair_id);
  }
  if (sqlite3_step(stmt) == SQLITE_DONE) {
    std::cout << "Repair deleted: " << repair_id << std::endl;
  }
  else {
    std::cout << "Error during deleting a repair: " << sqlite3_errmsg(db_ptr) << " " << sqlite3_errcode(db_ptr) << std::endl;
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db_ptr);
}

std::string GetStringFromColumn(sqlite3_stmt* stmt, int column_index, const std::string& default_value = "") {
  const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, column_index));
  return text ? std::string(text) : default_value;
}
RepairsSort Database::RetreiveRepairsByDate(std::string* date_1, int date_direction, std::string* date_2, int state, int asc_desc, int order) {
  //printf("RetreiveRepairsByDate is running with VARIANT: %d STATE: %d DIRECTION: %d ORDER: %d\n", date_direction, state, asc_desc, order);
  sqlite3* db_ptr = PtrDB();
  sqlite3_stmt* stmt;

  std::string query = "SELECT r.*, c.category, m.model, co.color, rs.repair_state, cu.*, r.date, cd.* FROM repairs r "
    "LEFT JOIN categories c ON r.category_id = c.category_id "
    "LEFT JOIN models m ON r.model_id = m.model_id "
    "LEFT JOIN colors co ON r.color_id = co.color_id "
    "LEFT JOIN repair_states rs ON r.repair_state_id = rs.repair_state_id "
    "LEFT JOIN customers cu ON r.customer_id = cu.customer_id "
    "LEFT JOIN custom_device cd ON r.cust_device_id = cd.cust_device_id";

  std::unordered_map<int, std::string> queries = {
    {0, " WHERE DATE(r.date) <= datetime('now', 'localtime')"},
    {1, " WHERE DATE(r.date) <= DATE(?)"}, // after this date
    {2, " WHERE DATE(r.date) >= DATE(? )"}, // before this date
    {3, " WHERE DATE(r.date) BETWEEN ? AND ?"}, // before this date
  };
  if(date_1)
    query += queries[date_direction];
  else {
    query += queries[0];
  }
  if (state == 8) {
    query += "";
  }
  if (state != 8) {
    query += " AND r.repair_state_id = ?";
  }

  std::unordered_map<int, std::string> order_by = {
  {0, "repair_id"},
  {1, "COALESCE(m.model, cd.model)"},
  {2, "c.category"},
  {4, "r.price"},
  {10, "r.date"}
  };
  std::unordered_map<int, std::string> sort_order = {
  {1, "DESC"},
  {2, "ASC"},
  };

  query += " ORDER BY " + order_by[order] + " " + sort_order[asc_desc];


  RepairsSort retreived;
  std::string date_1_str, date_2_str;


  if (sqlite3_prepare_v2(db_ptr, query.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
    std::string sum_query = "RUN WITH: ";
    if (date_1) {
      date_1_str = *date_1 + " 00:00:00";
      sqlite3_bind_text(stmt, 1, date_1_str.c_str(), -1, SQLITE_STATIC);
      sum_query += "DATE_1 ";
      if (date_2) {
        date_2_str = *date_2 + " 00:00:00";
        sqlite3_bind_text(stmt, 2, date_2_str.c_str(), -1, SQLITE_STATIC);
        sum_query += "DATE_2 ";
        if (state != 0) {
          sqlite3_bind_int(stmt, 3, state);
          sum_query += "STATE-BIND-3 ";

        }
      }
      else {
        if (state != 0) {
          sqlite3_bind_int(stmt, 2, state);
          sum_query += "STATE-BIND-2 ";

        }
      }
    }
    else {
      if (state != 0) {
        sqlite3_bind_int(stmt, 1, state);
        sum_query += "STATE-BIND-1 ";
      }
    }
    //printf("%s\n", sum_query.c_str());
    printf("%s\n", query.c_str());

    while (sqlite3_step(stmt) == SQLITE_ROW) {
      int repair_id = sqlite3_column_int(stmt, 0);
      int customer_id = sqlite3_column_int(stmt, 1);
      int repair_model_id = sqlite3_column_int(stmt, 2);
      int repair_category_id = sqlite3_column_int(stmt, 3);
      int repair_color_id = sqlite3_column_int(stmt, 4);
      std::string visible_note = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
      std::string hidden_note = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
      double price = sqlite3_column_double(stmt, 7);

      std::string date_str = GetStringFromColumn(stmt, 9);
      std::string sn_imei = GetStringFromColumn(stmt, 10);
      std::string device_name = GetStringFromColumn(stmt, repair_model_id > 0 ? 13 : 24);
      std::string color = GetStringFromColumn(stmt, repair_color_id > 0 ? 14 : 25);
      Device device(device_name, color); 
      std::string category = GetStringFromColumn(stmt, repair_category_id > 0 ? 12 : -1, "N/A");
      std::string state = GetStringFromColumn(stmt, 15);

      std::string customer_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));
      std::string customer_surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 18));
      std::string customer_email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 19));
      std::string customer_phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 20));
      Customer customer(customer_phone, customer_name, customer_surname, customer_email);

      Repair repair(customer, device, category, price, visible_note, hidden_note, sn_imei, state, date_str);
      //printf("Repair ID: %d = %.2f\n", repair_id, price);
      retreived.repairs.emplace(repair_id, repair);
      retreived.repairs_order.emplace_back(repair_id);
      retreived.total += price;
    }  
  }
  else {
    std::cout << "Error during retreiving a repair: " << sqlite3_errmsg(db_ptr) << " " << sqlite3_errcode(db_ptr) << std::endl;
  }
  sqlite3_finalize(stmt);
  sqlite3_close(db_ptr);
  //printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  return retreived;
}


