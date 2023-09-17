#include <iostream>
#include <fstream>
#include "json.hpp"
#include "customer_data.h"

using json = nlohmann::json;

// Initialize the static member variable
json CustomerData::customerDataArray = json();

// Member function to load customer data from a JSON file
void CustomerData::LoadCustomerData(const std::string& filename) {
    std::ifstream inputFile(filename);

    if (inputFile.is_open()) {
        // Check the file size
        inputFile.seekg(0, std::ios::end);
        std::streampos fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);

        if (fileSize > 0) {
            // The file is not empty, so proceed with parsing
            inputFile >> customerDataArray;
            inputFile.close();
            std::cout << "Loaded customer data successfully from file: " << filename << std::endl;
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

    std::cout << "WORK HERE" << std::endl;
    customerDataArray.push_back(data);
    std::cout << "WORK HERE" << std::endl;
}

// Member function to save customer data to the JSON file
void CustomerData::SaveCustomerData(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        outputFile << customerDataArray.dump(4); // Pretty formatting with 4-space indentation
        outputFile.close();
    }
}
