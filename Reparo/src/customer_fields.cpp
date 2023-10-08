#include "customer_fields.h"
#include <string.h> 

void CustomerPopulate::PopulteCustomerFields(std::vector<InputField>& fields, Customer& customer) {
		strncpy_s(fields[0].buffer, fields[0].bufferSize, customer.phone_number.c_str(), _TRUNCATE);
		strncpy_s(fields[1].buffer, fields[1].bufferSize, customer.name.c_str(), _TRUNCATE);
		strncpy_s(fields[2].buffer, fields[2].bufferSize, customer.surname.c_str(), _TRUNCATE);
		strncpy_s(fields[3].buffer, fields[3].bufferSize, customer.email.c_str(), _TRUNCATE);

}