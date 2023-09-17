#include "inputs_handler.h"
#include "customer_data.h"


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
            std::cout << "Last element does not have an 'ID' key." << std::endl;
        }
    }
    else {
        std::cout << "The customer data is empty." << std::endl;
        // Set the "ID" key to 1 in the lastElement JSON object
        json lastElement;
        lastElement["ID"] = 1;
        return 1;  // Or return lastElement["ID"];
    }
}

void CustomerDataInJson(json passedCustomerData) {

    CustomerData::LoadCustomerData("customer_data.json");
    passedCustomerData["ID"] = FindLastID();
    CustomerData::AddCustomerData(passedCustomerData);
    CustomerData::SaveCustomerData("customer_data.json");

}

void InputsHandler::HandleInputsFromFields(const InputField* fields, size_t numFields) {
    json customerData;
    for (size_t i = 0; i < numFields; ++i) {
        const InputField& field = fields[i];
        const char* fieldName = field.label + 2; // Skip "##" prefix in label
        const char* fieldValue = field.buffer;
        customerData[fieldName] = fieldValue;
    }
    
    CustomerDataInJson(customerData);
}
  