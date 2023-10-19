#include "sql_queries.h"
#include <iostream>

void SQLQuery::AllFromTable(const char* query, std::vector<std::string>& vector) {
    partsStock.OpenPartsStockDb();
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(partsStock.db, query, -1, &stmt, nullptr);
    //ex. "SELECT brand FROM brands";
    if (rc != SQLITE_OK) {
        // Add logging / Crash report
    }
    else {
        vector.clear(); // Clear the vector if needed

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* record = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            vector.emplace_back(record);
        }
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    sqlite3_close(partsStock.db);
}

void SQLQuery::OnID(const char* query, std::vector<std::string>& vector, int id) {
    // ex.  "SELECT model FROM models WHERE brand_id = ?";
    partsStock.OpenPartsStockDb();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(partsStock.db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        vector.clear(); // so it is not written on top of previous data
        // Execute the query
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* record = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            vector.emplace_back(record);
        }

        sqlite3_finalize(stmt);
    }
    else {
        // Add logging / Crash report
    }
    sqlite3_close(partsStock.db);
}

int SQLQuery::GetIdForValue(std::string query, const char* searchValue) {
    partsStock.OpenPartsStockDb();
    // Construct the SQL query as a string
    //ex. "SELECT " + std::string(columnName) + "_id FROM " + std::string(tableName) + " WHERE " + std::string(columnName) + " = ? ";
    std::string sql = query;
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(partsStock.db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        // Error logging / Crash report
        sqlite3_close(partsStock.db);
        return -1; // Return an error code or handle the error appropriately
    }

    // Bind the search value parameter to the placeholder
    rc = sqlite3_bind_text(stmt, 1, searchValue, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        // Error logging / Crash report

        sqlite3_finalize(stmt);
        sqlite3_close(partsStock.db);
        return -1; // Return an error code or handle the error appropriately
    }

    int id = -1; // Default value if the value is not found

    // Execute the query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Retrieve the ID from the result set
        id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(partsStock.db);

    return id;
}

std::set<std::string> TokenizeAndStore(const std::string& input) {
    std::set<std::string> words;
    std::istringstream iss(input);
    std::string word;

    while (iss >> word) {
        words.insert(word);
    }
    return words;
}

int SQLQuery::SearchForSimilarRecords(Part part) {
    partsStock.OpenPartsStockDb();
    std::set<std::string> words1 = TokenizeAndStore(part.quality.desc);
    const char* sqlQuery = "SELECT part_id, quality FROM parts WHERE brand_id = ? AND model_id = ? AND category_id = ? AND color = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(partsStock.db, sqlQuery, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the parameter values to the placeholders in the query

        sqlite3_bind_int(stmt, 1, part.brand.IDinDB);
        sqlite3_bind_int(stmt, 2, part.model.IDinDB);
        sqlite3_bind_int(stmt, 3, part.category.IDinDB);
        sqlite3_bind_int(stmt, 4, part.color.IDinDB);

        // Execute the query
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Retrieve data from the result set
            int partId = sqlite3_column_int(stmt, 0);
            const char* quality = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::set<std::string> words2 = TokenizeAndStore(quality);
            if (words1 == words2) {
                std::cout << "TRUE FOR: " << quality << " " << partId << std::endl;

                sqlite3_finalize(stmt);
                sqlite3_close(partsStock.db);
                return partId;
            }
            std::cout << " Quality: " << quality << std::endl;
        }

        // Finalize the statement
        sqlite3_finalize(stmt);
        sqlite3_close(partsStock.db);
        return 0;
    }
    else {
        // Handle the error
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(partsStock.db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(partsStock.db);
    return 0;

}

void SQLQuery::Update(int& rowToUpdate) {  // Update parts quantity
    partsStock.OpenPartsStockDb();

    std::string updateQuery = "UPDATE parts SET quantity = quantity + 1 WHERE part_id = ?";
    sqlite3_stmt* updateStmt;
    if (sqlite3_prepare_v2(partsStock.db, updateQuery.c_str(), -1, &updateStmt, NULL) == SQLITE_OK) {
        // Bind the parameter values to the placeholders in the query
        sqlite3_bind_int(updateStmt, 1, rowToUpdate);

        // Execute the update query
        int rc = sqlite3_step(updateStmt);
        if (rc == SQLITE_DONE) {
            // Update successful
            std::cout << "Quantity increased by 1 for matching record." << std::endl;
        }
        else {
            std::cerr << "Error updating record: " << sqlite3_errmsg(partsStock.db) << std::endl;
        }
        // Finalize the update statement
        sqlite3_finalize(updateStmt);
        sqlite3_close(partsStock.db);
    }
    else {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(partsStock.db) << std::endl;
    }
    return;
}

void SQLQuery::InsertPart(Part& part) {  
    partsStock.OpenPartsStockDb();
    const char* sqlInsert = "INSERT INTO parts (model_id, brand_id, category_id, color, quality, quantity) VALUES (?, ?, ?, ?, ?, 1)";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(partsStock.db, sqlInsert, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL prepare error: %s\n", sqlite3_errmsg(partsStock.db));
        sqlite3_close(partsStock.db);
        return;
    }

    // Bind parameters to the placeholders
    sqlite3_bind_int(stmt, 1, part.model.IDinDB);
    sqlite3_bind_int(stmt, 2, part.brand.IDinDB);
    sqlite3_bind_int(stmt, 3, part.category.IDinDB);
    sqlite3_bind_int(stmt, 4, part.color.IDinDB);
    sqlite3_bind_text(stmt, 5, part.quality.desc.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL step error: %s\n", sqlite3_errmsg(partsStock.db));
    }
    else {
        // Insert successful
        std::cout << "Part added to stock." << std::endl;
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(partsStock.db);
}

int SQLQuery::SearchForCustomerSQL(Customer customer) {
    if (customer.phone_number.empty())
        return -2;
    partsStock.OpenPartsStockDb();
    std::set<std::string> words1 = TokenizeAndStore(customer.phone_number);
    const char* sqlQuery = "SELECT customer_id, phone FROM customers WHERE phone = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(partsStock.db, sqlQuery, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the parameter values to the placeholders in the query
        const char* phone_number = customer.phone_number.c_str();
        sqlite3_bind_text(stmt, 1, phone_number, strlen(phone_number), NULL);
        // Execute the query
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Retrieve data from the result set
            int customerid = sqlite3_column_int(stmt, 0);
            const char* quality = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

            std::set<std::string> words2 = TokenizeAndStore(quality);

            if (words1 == words2) {
                std::cout << "TRUE FOR CUSTOMER: " << customerid << std::endl;
                sqlite3_finalize(stmt);
                sqlite3_close(partsStock.db);
                return customerid;
            }
        }
        // Finalize the statement
        sqlite3_finalize(stmt);
        sqlite3_close(partsStock.db);
        return 0;
    }
    else {
        // Handle the error
        std::cerr << "Error preparing SQL statement Search For Customers: " << sqlite3_errmsg(partsStock.db) << std::endl;
        sqlite3_close(partsStock.db); // Close the database in case of an error
        return -1;
    }
}

int SQLQuery::InsertCustomer(Customer customer) {
    int lastInsertedID = -1;
    partsStock.OpenPartsStockDb();

    // Prepare the SQL INSERT statement with placeholders
    std::string sqlInsert = "INSERT INTO customers (name, surname, email, phone) VALUES (?, ?, ?, ?)";

    // Prepare the statement
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(partsStock.db, sqlInsert.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(partsStock.db));
        sqlite3_finalize(stmt); // Clean up the statement
        sqlite3_close(partsStock.db);
        return 0; // Exit if an error occurred
    }

    // Bind values to the placeholders
    sqlite3_bind_text(stmt, 1, customer.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, customer.surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, customer.email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, customer.phone_number.c_str(), -1, SQLITE_STATIC);

    // Execute the statement
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL execution error: %s\n", sqlite3_errmsg(partsStock.db));
        return 0;
    }
    else {
       // Insertion successful
        lastInsertedID  = sqlite3_last_insert_rowid(partsStock.db);
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(partsStock.db);
    return lastInsertedID;
}

void SQLQuery::UpdateCustomer(int& rowToUpdate, Customer customerEdit) {
    partsStock.OpenPartsStockDb();

    std::string updateQuery = "UPDATE customers SET name = ?, surname = ?, email=?, phone=? WHERE customer_id = ?";
    sqlite3_stmt* updateStmt;
    if (sqlite3_prepare_v2(partsStock.db, updateQuery.c_str(), -1, &updateStmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(updateStmt, 5, rowToUpdate);
        sqlite3_bind_text(updateStmt, 1, customerEdit.name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(updateStmt, 2, customerEdit.surname.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(updateStmt, 3, customerEdit.email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(updateStmt, 4, customerEdit.phone_number.c_str(), -1, SQLITE_STATIC);


        // Execute the update query
        int rc = sqlite3_step(updateStmt);
        if (rc == SQLITE_DONE) {
            // Update successful
            std::cout << "Customer sucessfully updated." << std::endl;
        }
        else {
            std::cerr << "Error updating record: " << sqlite3_errmsg(partsStock.db) << std::endl;
        }
        // Finalize the update statement
        sqlite3_finalize(updateStmt);
        sqlite3_close(partsStock.db);
    }
    else {
        std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(partsStock.db) << std::endl;
    }
    return;
}

void SQLQuery::MatchingCustomers(std::string& partial_phone_number, std::unordered_map<int, Customer>& customers) {
    partsStock.OpenPartsStockDb();
    // Define the SQL query with a parameterized query.
    const char* query = "SELECT * FROM customers WHERE name LIKE ? OR surname LIKE ? OR email LIKE ? OR phone LIKE ?";

    // Prepare the SQL statement.
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(partsStock.db, query, -1, &stmt, NULL) == SQLITE_OK) {
        std::string phonePattern = "%" + partial_phone_number + "%";
        customers.clear();
        sqlite3_bind_text(stmt, 1, phonePattern.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, phonePattern.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, phonePattern.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, phonePattern.c_str(), -1, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Customer customer;
            customer.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            customer.surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            customer.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            customer.phone_number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            customers[(sqlite3_column_int(stmt, 0))] = customer;
           
        }
    }
    else {
        // Handle the error
        std::cerr << "Error preparing SQL statement MatchingCustomers: " << sqlite3_errmsg(partsStock.db) << std::endl;
        sqlite3_close(partsStock.db); // Close the database in case of an error
    }
    sqlite3_finalize(stmt);
    sqlite3_close(partsStock.db);
}

void SQLQuery::MatchingModels(std::string& model_query, std::vector<std::string>& vector, std::string label) {
    partsStock.OpenPartsStockDb();

    const char* query = nullptr;  // Declare the query variable

    if (label == "models") {
        query = "SELECT model FROM models WHERE model LIKE ?";
    }
    else if (label == "categories") {
        query = "SELECT category FROM categories WHERE category LIKE ?";
    }
    else {
        return;
    }

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(partsStock.db, query, -1, &stmt, NULL) == SQLITE_OK) {

        std::string like_query = "%" + model_query + "%";
        sqlite3_bind_text(stmt, 1, like_query.c_str(), -1, SQLITE_STATIC);

        vector.clear();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            vector.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        }
        for (size_t i = 0; i < vector.size(); i++) {
            std::cout << vector[i] << std::endl;
        }
    }
    else {
        std::cerr << "Error preparing SQL statement MatchingModels: " << sqlite3_errmsg(partsStock.db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(partsStock.db);
}

void SQLQuery::AddRepair(Repair& repair, int customerID) {
    int lastInsertedID = -1;
    partsStock.OpenPartsStockDb();


    // Prepare the SQL INSERT statement with placeholders
    std::string sqlInsert = "INSERT INTO repairs"
        "(customer_id, model_id, category_id, color_id, repair_desc, repair_desc_hidden, price, repair_state_id) "
        "VALUES(?, ?, ?, ?, ?, ?, ?, ?)";


    // Prepare the statement
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(partsStock.db, sqlInsert.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(partsStock.db));
        sqlite3_finalize(stmt); // Clean up the statement
        sqlite3_close(partsStock.db);
    }

    // Bind values to the placeholders
    sqlite3_bind_int(stmt, 1, customerID);
    sqlite3_bind_int(stmt, 2, repair.model.IDinDB);
    sqlite3_bind_int(stmt, 3, repair.category.IDinDB);
    sqlite3_bind_int(stmt, 4, repair.color.IDinDB);
    sqlite3_bind_text(stmt, 5, repair.note.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, repair.note_hidden.c_str(), -1, SQLITE_STATIC);
    std::cout << repair.price << std::endl;
    sqlite3_bind_double(stmt, 7, repair.price);
    sqlite3_bind_int(stmt, 8, repair.state.IDinDB);

    // Execute the statement
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL execution error repair: %s\n", sqlite3_errmsg(partsStock.db));

    }
    else {
        std::cout << "Repair added success!" << std::endl;
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(partsStock.db);

}

std::vector<Repair> SQLQuery::GetAllToDoRepairs() {
    partsStock.OpenPartsStockDb();
    std::vector<Repair> repairs;
    const char* query = "SELECT r.*, c.category, m.model, co.color, rs.repair_state, cu.* FROM repairs r "
        "LEFT JOIN categories c ON r.category_id = c.category_id "
        "LEFT JOIN models m ON r.model_id = m.model_id "
        "LEFT JOIN colors co ON r.color_id = co.color_id "
        "LEFT JOIN repair_states rs ON r.repair_state_id = rs.repair_state_id "
        "LEFT JOIN customers cu ON r.customer_id = cu.customer_id "
        "WHERE r.repair_state_id = 1";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(partsStock.db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(partsStock.db) << std::endl;
        return {};
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Repair repair;
        repair.id = sqlite3_column_int(stmt, 0);
        int customer_id = sqlite3_column_int(stmt, 1);
        repair.model.IDinDB = sqlite3_column_int(stmt, 2);
        repair.category.IDinDB = sqlite3_column_int(stmt, 3);
        repair.color.IDinDB = sqlite3_column_int(stmt, 4);
        repair.note = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        repair.note_hidden = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        repair.price = sqlite3_column_double(stmt, 7);

        // Additional data from related tables
        repair.category.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
        repair.model.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
        repair.color.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
        repair.state.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
        repair.customer.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 14));
        repair.customer.surname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));
        repair.customer.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 16));
        repair.customer.phone_number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));


        repairs.emplace_back(repair);


        //std::cout << "Note: " << repair.note << std::endl;
        //std::cout << "Category: " << repair.category.name << std::endl;
        //std::cout << "Model: " << repair.model.name << std::endl;
        //std::cout << "Color: " << repair.color.name << std::endl;
        //std::cout << "State: " << repair.state.name << std::endl;
        //std::cout << "Customer: " << repair.customer.name << std::endl;
    }
    return repairs;
    sqlite3_finalize(stmt);
    sqlite3_close(partsStock.db);
}


void SQLQuery::Prices() {
    partsStock.OpenPartsStockDb();
    const char* query = "SELECT price FROM repairs";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(partsStock.db, query, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(partsStock.db) << std::endl;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        double price = sqlite3_column_double(stmt, 0); // Index 0 for the "price" column
        std::cout << "Price: " << price << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(partsStock.db);
}