#include "database.h"

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

int Database::QueryCustomerByPhone(std::string phone) {
    std::cout << "QueryCustomerByPhone is running  " << std::endl;
    static int result;
    OpenDB();
    if (phone.empty()) { result = -2; }
    const char* query = "SELECT customer_id FROM customers WHERE phone = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, phone.c_str(), phone.size(), NULL);
        int customerID;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            customerID = sqlite3_column_int(stmt, 0);
            result =  customerID; // ID of existing customer
        }
        result =  0; // For new customer
    }
    else {
        result =  -1; // Error querying db
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
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
    {"##Phone", "SELECT phone, name FROM customers WHERE phone LIKE ?"},
    };
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, queries[label], -1, &stmt, NULL) == SQLITE_OK) {
        std::string buffer_str = buffer;
        std::string like_query = "%" + buffer_str + "%";
        sqlite3_bind_text(stmt, 1, like_query.c_str(), -1, SQLITE_STATIC);

        attribute.data.clear();
        int record_count = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (record_count == 5) {
                break;
            }
            attribute.data.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
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
    static int model_id = GetIDForValue("##Model", repair.device.name);
    static int category_id = GetIDForValue("##Category", repair.category);
    static int color_id = GetIDForValue("##Color2", repair.device.color);
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
        sqlite3_bind_text(stmt, 5, repair.visible_note, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, repair.hidden_note, -1, SQLITE_STATIC);
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
