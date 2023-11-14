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
}

int Database::QueryCustomerByPhone(std::string phone) {
    OpenDB();
    if (phone.empty()) { return -2; }
    const char* query = "SELECT customer_id FROM customers WHERE phone = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, phone.c_str(), phone.size(), NULL);
        int customerID;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            customerID = sqlite3_column_int(stmt, 0);
            return customerID; // ID of existing customer
        }
        return 0; // For new customer
    }
    else {
        return -1; // Error querying db
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void Database::InsertCustomer(Customer& customer) {
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
        std::cout << "Customer added: " << customer.phone << " " << customer.name << std::endl;
    }
    else {
        std::cout << "Error during adding a customer" << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void Database::ManageSearchState(const char* label, Attribute& attribute, const char* buffer) {
    OpenDB();

    static std::map<const char*, const char*> queries = {
    {"##Model", "SELECT model FROM models WHERE model LIKE ?"},
    {"##Category", "SELECT category FROM categories WHERE category LIKE ?"},
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
    OpenDB();
    static std::map<const char*, const char*> queries = {
    {"##Color", "SELECT model_id FROM models WHERE LOWER(model) = LOWER(?)"},
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