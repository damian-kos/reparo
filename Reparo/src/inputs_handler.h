#pragma once

#ifndef INPUTS_HANDLER_H
#define INPUTS_HANDLER_H

#include <string>
#include "json.hpp" // Include the nlohmann/json library
#include "customer_input_window.h"  // Include the customer_input_window.h header to access InputField

using json = nlohmann::json;

class InputsHandler {
public:
    static void HandleInputsFromFields(const InputField* fields, int numFields);
private:
};

#endif // INPUTS_HANDLER_H
