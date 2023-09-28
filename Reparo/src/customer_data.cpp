#include <iostream>
#include <fstream>
#include "customer_data.h"


// Initialize the static member variable
json CustomerData::customerDataArray = json();

// Member function to load customer data from a JSON file
void CustomerData::LoadCustomerData(const std::string& filename) {
    std::ifstream inputFile(filename);

    if (inputFile.is_open()) {
        // Check the file size
        inputFile.seekg(0, std::ios::end);
        int fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);

        if (fileSize > 0) {
            // The file is not empty, so proceed with parsing
            inputFile >> customerDataArray;
            inputFile.close();
            //std::cout << "Loaded customer data successfully from file: " << filename << std::endl;
        }
        else {
            // The file is empty, no need to parse
            inputFile.close();
            std::cout << "File is empty: " << filename << std::endl;
        }
    }
    else {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
}
// Member function to access the loaded customer data
const json& CustomerData::GetCustomerData() {
 
        return customerDataArray;
}

// Member function to add customer data to the array
void CustomerData::AddCustomerData(const json& data) {
    bool found = false; // Flag to indicate if a record with the same ID was found
    std::cout << data << "NO ID" << std::endl;

    for (json& record : customerDataArray) {
        if (record["ID"] == data["ID"]) {
            std::cout << data << "found ID" << std::endl;

            // Replace the data with new data
            record["Email"] = data["Email"];
            record["Name"] = data["Name"];
            record["Surname"] = data["Surname"];
            record["PhoneNumber"] = data["PhoneNumber"];
            found = true; // Record with the same ID was found
            break; // Exit the loop since we don't need to search further
        }
    }

    if (!found) {
        std::cout << data << "NO found ID" << std::endl;

        // Add a new record to the array if no record with the same ID was found
        //customerDataArray.push_back(data);
    }
}

// Member function to save customer data to the JSON file
void CustomerData::SaveCustomerData(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        outputFile << customerDataArray.dump(4); // Pretty formatting with 4-space indentation
        outputFile.close();
    }
}
