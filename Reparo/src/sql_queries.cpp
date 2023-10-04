#include "sql_queries.h"

void SQLQuery::QueryAllFromTable(const char* query, std::vector<std::string>& vector) {

    partsStock.OpenPartsStockDb();
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(partsStock.db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
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