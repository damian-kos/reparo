#pragma once

#ifndef INPUTS_HANDLER_H
#define INPUTS_HANDLER_H

#include <string>
#include "json.hpp" // Include the nlohmann/json library
#include "input.h"

using json = nlohmann::json;

class InputsHandler {
public:

	static void HandleInputsFromFields(const InputField* fields, size_t numFields);

private:

};

#endif // INPUTS_HANDLER_H
