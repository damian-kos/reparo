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
        try {
            inputFile >> customerDataArray;
            inputFile.close();
        }
        catch (const json::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
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
    customerDataArray.push_back(data);
}

// Member function to save customer data to the JSON file
void CustomerData::SaveCustomerData(const std::string& filename) {
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        outputFile << customerDataArray.dump(4); // Pretty formatting with 4-space indentation
        outputFile.close();
    }
}
