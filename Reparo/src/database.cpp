#include "database.h"
#include <sstream>
Database::Database() {}

Database::~Database() {
    std::cout << "Database destroyed" << std::endl;
}

void Database::OpenDB() {
    db = nullptr;
    int rc = sqlite3_open("parts_stock.db", &db);
    if (rc != SQLITE_OK) {
        std::cout << "DATABASE CAN'T BE OPEN" << std::endl;
    }
    std::cout << "DB IS OPEN" << std::endl;
}

int Database::QueryCustomerIDByPhone(std::string phone) {
    std::cout << "QueryCustomerIDByPhone is running  " << std::endl;
    int result = -1;
    OpenDB();
    if (phone.empty()) { result = -2; }
    const char* query = "SELECT customer_id FROM customers WHERE phone = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, phone.c_str(), phone.size(), NULL);
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
    sqlite3_close(db);
    return result;
}

Customer Database::QueryCustomerByPhone(std::string phone) {
    Customer customer;
    std::string name;
    std::string surname;
    std::string email;
    std::cout << "QueryCustomerByPhone is running  " << std::endl;
    OpenDB();
    const char* query = "SELECT * FROM customers WHERE phone = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, phone.c_str(), phone.size(), NULL);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        }
        customer = Customer(phone, name, surname, email);
    }
    else {
        std::cerr << "Error preparing SQL statement QueryCustomerByPhone: " << sqlite3_errmsg(db) << std::endl;
        return customer; // Error querying db
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return customer;
}

void Database::InsertCustomer(Customer& customer, int* lastRowId) {
    std::cout << "InsertCustomer is running  " << std::endl;

    OpenDB();
    const char* query = "INSERT INTO customers (name, surname, email, phone) VALUES (?, ?, ?, ?)";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, customer.name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, customer.surname.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, customer.email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, customer.phone.c_str(), -1, SQLITE_STATIC);
    }

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (lastRowId != nullptr) {
            *lastRowId = sqlite3_last_insert_rowid(db);
        }
        std::cout << "Customer added: " << customer.phone << " " << customer.name << std::endl;
    }
    else {
        std::cout << "Error during adding a customer" << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void Database::ManageSearchState(const char* label, Attribute& attribute, const char* buffer) {
    std::cout << "ManageSearchState is running  " << std::endl;

    OpenDB();
    static std::map<const char*, const char*> queries = {
    {"##Model", "SELECT model FROM models WHERE model LIKE ?"},
    {"##Category", "SELECT category FROM categories WHERE category LIKE ?"},
    {"##Phone", "SELECT phone, name, surname FROM customers WHERE phone LIKE ?"},
    };
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, queries[label], -1, &stmt, NULL) == SQLITE_OK) {
        std::string buffer_str = buffer;
        std::string like_query = "%" + buffer_str + "%";
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
            if (!value.empty()) {
                value.pop_back();
            }

            attribute.data.emplace_back(value);

            record_count++;
        }
    }
    else {
        std::cerr << "Error preparing SQL statement ManageSearchState (3 param): " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

}

void Database::ManageSearchState(const char* label, Attribute& attribute, const char* buffer, int rel_id) {
    std::cout << "ManageSearchState 2 is running  " << std::endl;

    OpenDB();
    static std::map<const char*, const char*> queries = {
    {"##Color", "SELECT color FROM colors WHERE color_id IN (SELECT color_id FROM model_color WHERE model_id = ?)"},
    };
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, queries[label], -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, rel_id);
        attribute.data.clear();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            attribute.data.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        }
    }
    else {
        std::cerr << "Error preparing SQL statement ManageSearchState (4 param): " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int Database::GetIDForValue(const char* label, const char* value) {
    std::cout << "GetIDForValue is running  " << std::endl;

    OpenDB();
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

    if (sqlite3_prepare_v2(db, queries[label], -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, value, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            id = sqlite3_column_int(stmt, 0);
        }
        else {
            std::cerr << "GetIDForValue: No rows returned." << std::endl;
        }
    }
    else {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        id = -1; // error
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return id;
}

bool Database::GetBoolForValue(const char* label, const char* buffer) {
    std::cout << "GetBoolForValue is running  " << std::endl;

    OpenDB();
    static std::map<const char*, const char*> queries = {
    {"##Model", "SELECT model_id FROM models WHERE LOWER(model) = LOWER(?)"},
    {"##Category", "SELECT category_id FROM categories WHERE LOWER(category) = LOWER(?)"},
    {"##Color", "SELECT color_id FROM colors WHERE LOWER(color) = LOWER(?)"},
    };
    bool is_found = false;
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, queries[label], -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, buffer, -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
 
            is_found = true;
        }
        else {
            std::cerr << "GetBoolForValue: No rows returned. " << std::endl;
        }
    }
    else {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        is_found = false; // error
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return is_found;
}

void Database::InsertRepair(Repair repair) {
    std::cout << "InsertRepair is running  " << std::endl;
    static int customer_id = GetIDForValue("##Customer", repair.customer.phone.c_str());
    static int model_id = GetIDForValue("##Model", repair.device.name.c_str());
    static int category_id = GetIDForValue("##Category", repair.category.c_str());
    static int color_id = GetIDForValue("##Color2", repair.device.color.c_str());
    OpenDB();
    sqlite3_stmt* stmt; 
    const char* query = "INSERT INTO repairs"
        "(customer_id, model_id, category_id, color_id, repair_desc, repair_desc_hidden, price, repair_state_id) "
        "VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, customer_id);
        sqlite3_bind_int(stmt, 2, model_id);
        sqlite3_bind_int(stmt, 3, category_id);
        sqlite3_bind_int(stmt, 4, color_id);
        sqlite3_bind_text(stmt, 5, repair.visible_note.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, repair.hidden_note.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 7, repair.price);
        sqlite3_bind_int(stmt, 8, static_cast<int>(RepairState::TO_DO));
    }

    if (sqlite3_step(stmt) == SQLITE_DONE) {
       
        std::cout << "Repair added: " << repair.device.name << " " << repair.customer.phone << std::endl;
    }
    else {
        std::cout << "Error during adding a repair: " << sqlite3_errmsg(db) << " " << sqlite3_errcode(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

std::map<int, Repair> Database::RetreiveRepairsOfState(int state) {
    std::cout << "RetreiveRepairsOfState is running " << std::endl;
    OpenDB();
    std::map<int, Repair> repairs;
    sqlite3_stmt* stmt;
    const char* query = "SELECT r.*, c.category, m.model, co.color, rs.repair_state, cu.* FROM repairs r "
        "LEFT JOIN categories c ON r.category_id = c.category_id "
        "LEFT JOIN models m ON r.model_id = m.model_id "
        "LEFT JOIN colors co ON r.color_id = co.color_id "
        "LEFT JOIN repair_states rs ON r.repair_state_id = rs.repair_state_id "
        "LEFT JOIN customers cu ON r.customer_id = cu.customer_id "
        "WHERE r.repair_state_id = ?";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, state);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int repair_id = sqlite3_column_int(stmt, 0);
            int customer_id = sqlite3_column_int(stmt, 1);
            int repair_model_id = sqlite3_column_int(stmt, 2);
            int repair_category_id = sqlite3_column_int(stmt, 3);
            int repair_color_id = sqlite3_column_int(stmt, 4);
            std::string visible_note = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            std::string hidden_note = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
            double price = sqlite3_column_double(stmt, 7);

            //// Additional data from related tables
            std::string device_name = (repair_model_id > 0) ?
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10)) : "N/A";
            std::string color = (repair_color_id> 0) ?
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11)) : "N/A";
            Device device(device_name, color);
            std::string category = (repair_category_id > 0) ?
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9)) : "N/A";
            std::string state = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
          
            
            std::string customer_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 14));
            std::string customer_surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));
            std::string customer_email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 16));
            std::string customer_phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));
            Customer customer(customer_phone, customer_name, customer_surname, customer_email);

            Repair repair(customer, device, category, price, visible_note, hidden_note, state);
         
            repairs.insert(std::make_pair(repair_id, repair));;
        }
    }
   
    /*if (sqlite3_step(stmt) == SQLITE_DONE) {
        std::cout << "Repairs retreived for state: " << state << std::endl;
    }*/
    else {
        std::cout << "Repairs couldn't be retreived " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return repairs;
}

std::map<int, std::string> Database::GetRepairStates() {
    std::cout << "GetRepairsStates is running " << std::endl;
    OpenDB();
    static std::map<int, std::string> states;
    sqlite3_stmt* stmt;
    const char* query = "SELECT * FROM repair_states";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
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
    sqlite3_close(db);
    return states;
}

void Database::UpdateCustomer(Customer& customer, int id) {
    std::cout << "UpdateCustomer is running" << std::endl;
    OpenDB();
    sqlite3_stmt* stmt;
    const char* query = "UPDATE customers SET name = ?, surname = ?, email = ?, phone = ? WHERE customer_id = ?";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        std::cout << "Update will happen" << std::endl;
        std::cout << id << " / " << customer.name.c_str() << " / " << customer.surname.c_str() << " /  " << customer.email.c_str() << " / " << customer.phone.c_str() << std::endl;
        sqlite3_bind_text(stmt, 1, customer.name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, customer.surname.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, customer.email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, customer.phone.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error updating customer: " << sqlite3_errmsg(db) << std::endl;
        }
    }
    else {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void Database::UpdateRepair(Repair& repair, int id) {
    std::cout << "UpdateRepair is running" << std::endl;
    static int model_id = GetIDForValue("##Model", repair.device.name.c_str());
    static int category_id = GetIDForValue("##Category", repair.category.c_str());
    static int color_id = GetIDForValue("##Color2", repair.device.color.c_str());
    OpenDB();
    sqlite3_stmt* stmt;
    const char* query = "UPDATE repairs SET model_id = ?, category_id = ?, color_id = ?, repair_desc = ?, repair_desc_hidden = ?, price = ?, repair_state_id = ? WHERE repair_id = ?";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        std::cout << "Update Repairwill happen" << std::endl;
        //std::cout << id << " / " << customer.name.c_str() << " / " << customer.surname.c_str() << " /  " << customer.email.c_str() << " / " << customer.phone.c_str() << std::endl;
        sqlite3_bind_int(stmt, 1, model_id);
        sqlite3_bind_int(stmt, 2, category_id);
        sqlite3_bind_int(stmt, 3, color_id);
        sqlite3_bind_text(stmt, 4, repair.visible_note.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, repair.hidden_note.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 6, repair.price);
        std::cout << "Repair State: |" << repair.state << "|" << std::endl;
        int state_id = GetIDForValue("##State", repair.state.c_str());
        sqlite3_bind_int(stmt, 7, state_id);
        sqlite3_bind_int(stmt, 8, id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error updating repair: " << sqlite3_errmsg(db) << std::endl;
        }
    }
    else {
        std::cerr << "Error preparing UpdateRepair statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int Database::GetIDForID(int id, std::string table) {
    int found_id = -1;
    std::cout << "GetIDForValue is running " << std::endl;
    static std::unordered_map<std::string, const char*> queries = {
    {"repairs", "SELECT customer_id FROM repairs WHERE repair_id = ?"},
    };
    OpenDB();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, queries[table], -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            found_id = sqlite3_column_int(stmt, 0);
        }
        else {
            std::cerr << "GetIDForID: No rows returned." << std::endl;
        }
    }
    else {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        found_id = -1; // error
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return found_id;
}
