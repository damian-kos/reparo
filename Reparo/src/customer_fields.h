#pragma once

#include "structs.h"
#include "sql_queries.h"



class CustomerPopulate {
public:
    void PopulteCustomerFields(std::vector<InputField>& fields, Customer& customer);
    int Submit(Customer& cust);
};