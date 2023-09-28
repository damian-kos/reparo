#include "inputs_handler.h"
#include "customer_data.h"
#include <iostream>

using json = nlohmann::json;

int FindLastID() {
    

    json customerDataJson = CustomerData::GetCustomerData();

    if (!customerDataJson.empty()) {
        json lastElement = customerDataJson.back();
        if (lastElement.find("ID") != lastElement.end()) {
            int lastElementID = lastElement["ID"];
            lastElementID++;
            return lastElementID;
        }
        else {

        }
    }
    else {

        // Set the "ID" key to 1 in the lastElement JSON object
        json lastElement;
        lastElement["ID"] = 1;
        return 1;  // Or return lastElement["ID"];
    }
}

void CustomerDataInJson(json& passedCustomerData) {

    CustomerData::LoadCustomerData("customer_data.json");
    CustomerData::AddCustomerData(passedCustomerData);
    CustomerData::SaveCustomerData("customer_data.json");

}
bool CheckForEmptyField(const char* buffer) {
    if (std::strlen(buffer) > 2) {
        return true;
    }
    else {
        return false;
    }

        
}

bool InputsHandler::HandleInputsFromFields(const InputField* fields, int numFields, const int* id) {

    json customerData;

    for (int i = 0; i < numFields; ++i) {
        const InputField& field = fields[i];
        const char* fieldName = field.label + 2; // Skip "##" prefix in label
        const char* fieldValue = field.buffer;
        customerData[fieldName] = fieldValue;
        if (CheckForEmptyField(field.buffer) == false) {

           return false;
        }
    }

    if (id == nullptr) {
        customerData["ID"] = FindLastID();
        std::cout << customerData << " NO ID" <<  std::endl;
    }
    else {
        std::cout << customerData << " WITH  ID" << std::endl;
        customerData["ID"] = *id;
    }
    CustomerDataInJson(customerData);
    return true;
}
 
