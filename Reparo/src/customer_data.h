#ifndef CUSTOMER_DATA_H
#define CUSTOMER_DATA_H

#include <string>
#include "json.hpp"

using json = nlohmann::json;

class CustomerData {
public:
    // Member function to load customer data from a JSON file
    static void LoadCustomerData(const std::string& filename);

    static const json& GetCustomerData();
    static void AddCustomerData(const json& data);
    static void SaveCustomerData(const std::string& filename);



private:
    static json customerDataArray;
    static json brandsArray;

};

#endif // CUSTOMER_DATA_H
