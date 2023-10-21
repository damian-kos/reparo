#include "customer_fields.h"
#include <string.h> 
#include <iostream>

void CustomerPopulate::PopulteCustomerFields(std::vector<InputField>& fields, Customer& customer) {
		for (InputField& field : fields) {
			memset(field.buffer, 0, field.bufferSize); // Set all characters to null (clear the buffer)
		}
		strncpy_s(fields[0].buffer, fields[0].bufferSize, customer.phone_number.c_str(), _TRUNCATE);
		strncpy_s(fields[1].buffer, fields[1].bufferSize, customer.name.c_str(), _TRUNCATE);
		strncpy_s(fields[2].buffer, fields[2].bufferSize, customer.surname.c_str(), _TRUNCATE);
		strncpy_s(fields[3].buffer, fields[3].bufferSize, customer.email.c_str(), _TRUNCATE);
}

int CustomerPopulate::Submit(Customer& cust) {
	SQLQuery sql;
	int result = sql.SearchForCustomerSQL(cust);
	if (cust.phone_number.empty()) {
		std::cout << "SUBMIT 0" << std::endl;
		return -2;
	}
	if (result == 0) {
		std::cout << "SUBMIT 1" << std::endl;
		return 0;
	}
	if (result == -1) {
		std::cout << "SUBMIT 2" << std::endl;
		return -1;
	}
	else {
		return result;
	}
}