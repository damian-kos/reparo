#include "sql_queries.h"
#include <iostream>

void SQLQuery::AllFromTable(const char* query, std::vector<std::string>& vector) {
    partsStock.OpenPartsStockDb();
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(partsStock.db, query, -1, &stmt, nullptr);
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

    partsStock.OpenPartsStockDb();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(partsStock.db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind the id parameter to the placeholder in query
        sqlite3_bind_int(stmt, 1, id);
        vector.clear();
        // Execute the query
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Retrieve the model data from the result set
            const char* record = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            vector.emplace_back(record);
        }

        // Finalize the statement after use
        sqlite3_finalize(stmt);
    }
    else {
        // Add logging / Crash report
    }
    sqlite3_close(partsStock.db);
}

void SQLQuery::OnID(const char* query, std::vector<std::string>& vector) {

    partsStock.OpenPartsStockDb();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(partsStock.db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind the id parameter to the placeholder in query
       
        vector.clear();
        // Execute the query
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Retrieve the model data from the result set
            const char* record = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            vector.emplace_back(record);
        }

        // Finalize the statement after use
        sqlite3_finalize(stmt);
    }
    else {
        // Add logging / Crash report
    }
    sqlite3_close(partsStock.db);
}

int SQLQuery::GetIdForValue(const char* tableName, const char* columnName, const char* searchValue) {

    partsStock.OpenPartsStockDb();

    // Construct the SQL query as a string
    std::string sql = "SELECT " + std::string(columnName) + "_id FROM " + std::string(tableName) + " WHERE " + std::string(columnName) + " = ? ";
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
        int brandId = part.brand.current_id + 1;
        int modelId = part.model.current_id + 1;
        int categoryId = part.category.current_id + 1;
        int colorId = part.color.current_id + 1;

        sqlite3_bind_int(stmt, 1, brandId);
        sqlite3_bind_int(stmt, 2, modelId);
        sqlite3_bind_int(stmt, 3, categoryId);
        sqlite3_bind_int(stmt, 4, colorId);

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

void SQLQuery::Update(int& rowToUpdate) {
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

void SQLQuery::InsertPart(Part part) {
        partsStock.OpenPartsStockDb(); 

        //Construct the SQL query to insert data into the 'parts' table
        std::string sqlInsert = "INSERT INTO parts (model_id, brand_id, category_id, color, quality, quantity) VALUES (";
        sqlInsert += std::to_string(part.model.current_id + 1) + ", ";
        sqlInsert += std::to_string(part.brand.current_id + 1) + ", ";
        sqlInsert += std::to_string(part.category.current_id + 1) + ", ";
        sqlInsert += std::to_string(part.color.current_id + 1) + ", ";
        sqlInsert += "'" + part.quality.desc + "', "; // Assuming quality is TEXT
        sqlInsert += "1)"; // Assuming quantity is initialized to 0
        int rc = sqlite3_exec(partsStock.db, sqlInsert.c_str(), 0, 0, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(partsStock.db));
        }
        if (rc == SQLITE_DONE) {
            // Update successful
        }
        std::cout << "Part added to stock." << std::endl;

        sqlite3_close(partsStock.db);
}

int SQLQuery::SearchForCustomerSQL(Customer customer) {
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

    // Return 0 in case of an error
}


void SQLQuery::InsertCustomer(Customer customer) {
    partsStock.OpenPartsStockDb();
    std::cout << customer.name << " " << customer.phone_number << std::endl;
    ////Construct the SQL query to insert data into the 'parts' table
    std::string sqlInsert = "INSERT INTO customers (name, surname, email, phone) VALUES (";
    sqlInsert += "'" + customer.name + "', ";
    sqlInsert += "'" + customer.surname + "', ";
    sqlInsert += "'" + customer.email + "', ";
    sqlInsert += "'" + customer.phone_number + "')";

    int rc = sqlite3_exec(partsStock.db, sqlInsert.c_str(), 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(partsStock.db));
    }
    if (rc == SQLITE_DONE) {
       // Update successful
    }
    std::cout << "Customer added." << std::endl;

    sqlite3_close(partsStock.db);

}

void SQLQuery::MatchingCustomers(std::string& partial_phone_number, std::unordered_map<int, Customer>& customers) {
    partsStock.OpenPartsStockDb();
    // Define the SQL query with a parameterized query.
    const char* query = "SELECT * FROM customers WHERE name LIKE ? OR surname LIKE ? OR email LIKE ? OR phone LIKE ?";

    //std::cout << "WE ARE HERE " << std::endl;
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