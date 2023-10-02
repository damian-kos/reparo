#include "parts_stock.h"
#include <cstdio>
#include <iostream>

void PartsStock::OpenPartsStockDb() {
    // 1. Declare a pointer to an SQLite database connection
    db = nullptr;

    // 2. Open or create the SQLite database
    int rc = sqlite3_open("parts_stock.db", &db);

    if (rc != SQLITE_OK) {
        // Handle the error, e.g., throw an exception or report it
        std::cout << "DATABASE CAN'T BE OPEN" << std::endl;

    }
}

void PartsStock::CreateTable() {
    OpenPartsStockDb();
    //const char* createTableSQL = "CREATE TABLE IF NOT EXISTS brands (ID INTEGER PRIMARY KEY, Brand TEXT, Make TEXT, Model TEXT, Part TEXT, Color TEXT, Quality TEXT, Quantity INTEGER, Supplier TEXT, SupplierSKU TEXT, OwnSKU TEXT, BuyPriceIncVAT REAL, BuyPriceExcVAT REAL, Location TEXT, BoughtDate TEXT)";
    //const char* createPartTable = "CREATE TABLE IF NOT EXISTS part (part_id INTEGER PRIMARY KEY, brand_id INTEGER, model_id INTEGER, amount INTEGER, category INTEGER, category_id INTEGER, quality TEXT, color TEXT, FOREIGN KEY (model_id) REFERENCES models (model_id), FOREIGN KEY (category) REFERENCES categories (category_id))";


   
    //const char* createCategoryTable = "CREATE TABLE IF NOT EXISTS categories (category_id INTEGER PRIMARY KEY, category TEXT UNIQUE)";

   
    //const char* populateCategories = "INSERT INTO categories (category) VALUES  ('Screen'), ('Battery'), ('Back Cover'), ('Charge Port')";

   /* const char* populatePart = "INSERT INTO part (brand_id, model_id, amount, category, quality, color) SELECT m.brand_id, p.model_id, 10, 1, 'High', 'Red' FROM models AS m JOIN part AS p ON m.model_id = p.model_id WHERE m.model = 'iPhone 13 Pro Max'";*/


    // 3. Execute the SQL statement to create the table
   
    //rc = sqlite3_exec(db, createCategoryTable, 0, 0, 0);
    //rc = sqlite3_exec(db, createPartTable, 0, 0, 0);

   
    //rc = sqlite3_exec(db, populateModels , 0, 0, 0);
    //rc = sqlite3_exec(db, populateCategories, 0, 0, 0);

    const char* createBrandTable = "CREATE TABLE IF NOT EXISTS brands (brand_id INTEGER PRIMARY KEY, brand TEXT UNIQUE)";
    const char* createModelTable = "CREATE TABLE IF NOT EXISTS models (model_id INTEGER PRIMARY KEY, model TEXT UNIQUE, brand_id INTEGER, FOREIGN KEY (brand_id) REFERENCES brands (brand_id))";
    const char* createCategoriesTable = "CREATE TABLE IF NOT EXISTS categories (category_id INTEGER PRIMARY KEY, category TEXT UNIQUE)";
    const char* createPartsTable = "CREATE TABLE IF NOT EXISTS parts (part_id INTEGER PRIMARY KEY, brand_id INTEGER, model_id INTEGER, category_id INTEGER, color TEXT, quality TEXT, quantity INTEGER, FOREIGN KEY (model_id) REFERENCES models (model_id), FOREIGN KEY (category_id) REFERENCES categories (category_id))";

    const char* createColorsTable = "CREATE TABLE IF NOT EXISTS colors (color_id INTEGER PRIMARY KEY, color TEXT UNIQUE)";
    const char* createModelColorTable =
        "CREATE TABLE IF NOT EXISTS model_color ("
        "model_id INTEGER,"
        "color_id INTEGER,"
        "UNIQUE(model_id, color_id),"
        "FOREIGN KEY (model_id) REFERENCES models (model_id),"
        "FOREIGN KEY (color_id) REFERENCES colors (color_id));";

    const char* createQualityTable = "CREATE TABLE IF NOT EXISTS qualities (quality_id INTEGER PRIMARY KEY, quality TEXT UNIQUE)";
    


    int rc = sqlite3_exec(db, createBrandTable, 0, 0, 0);
    rc = sqlite3_exec(db, createModelTable, 0, 0, 0);
    rc = sqlite3_exec(db, createCategoriesTable, 0, 0, 0);
    rc = sqlite3_exec(db, createPartsTable, 0, 0, 0);
    rc = sqlite3_exec(db, createColorsTable, 0, 0, 0);
    rc = sqlite3_exec(db, createModelColorTable, 0, 0, 0);
    rc = sqlite3_exec(db, createQualityTable, 0, 0, 0);





    const char* populateBrands = "INSERT INTO brands (brand) VALUES ('Apple'), ('Samsung'), ('Xiaomi'), ('Google')";
    const char* populateModels = "INSERT INTO models (model, brand_id) VALUES  ('iPhone 13', 1)";
    const char* populateCategories = "INSERT INTO categories (category) VALUES ('Screen'), ('Battery'), ('Charge Port'), ('Back Cover')";
    const char* populateColors = "INSERT INTO colors (color) VALUES ('Starlight'), ('Midnight'), ('Blue'), ('Pink'), ('Cosmic Grey'), ('Cloud Blue'), ('Cloud Pink'), ('Cloud White'), ('Aura Red')";
    const char* popuulateQuality = "INSERT INTO qualities (quality) VALUES ('Pulled'), ('Refurbished'), ('Grade A'), ('Grade B'), ('Grade C'), ('Aftermarket'), ('Premium'), ('Budget'), ('Soft OLED'), ('Hard OLED'),('LCD')";

    const char* populateModelColors = "INSERT OR IGNORE INTO model_color (model_id, color_id) VALUES (5,5), (5,6), (5,7), (5,8), (3,9), (3,10), (3,11), (3,12), (3,13)";



    //const char* populatePart = "INSERT INTO parts (model_id, brand_id, category_id, color, quality) SELECT 2, brand_id, 1, 'black', 'genuine' FROM models WHERE model_id = 2";
    //rc = sqlite3_exec(db, populateBrands, 0, 0, 0);
    //rc = sqlite3_exec(db, populateModels, 0, 0, 0);
    //rc = sqlite3_exec(db, populateCategories, 0, 0, 0);
    //rc = sqlite3_exec(db, populatePart, 0, 0, 0);
    //rc = sqlite3_exec(db, populateColors, 0, 0, 0);
    rc = sqlite3_exec(db, populateModelColors, 0, 0, 0);
    rc = sqlite3_exec(db, popuulateQuality, 0, 0, 0);





    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }

    // Close the database connection (whether the table was created or not)
    sqlite3_close(db);
}

void PartsStock::PrintOutData(){
    OpenPartsStockDb();
    const char* selectResults = "SELECT b.brand, m.model FROM brands b INNER JOIN models m ON b.brand_id = m.brand_id";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, selectResults, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* brand = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)); // brand column
            const char* model = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)); // model column

            // Process the retrieved data, e.g., print it
            printf("%s %s\n", brand, model);
        }
    }

    sqlite3_finalize(stmt);


}

