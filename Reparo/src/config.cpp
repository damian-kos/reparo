#include "config.h"

RO_Config::RO_Config(){
  printf("RO_Config created\n");
}

json RO_Config::GetConfig()
{
  json data;

  // Try to read existing data from the file
  std::ifstream file_in("config.json");
  if (file_in.is_open()) {
    try {
      file_in >> data;
    }
    catch (const json::parse_error& e) {
      // Handle parse error (e.g., empty file)
      std::cerr << "Error parsing JSON: " << e.what() << std::endl;
      data = json::object();
    }
    file_in.close();
  }

  return data;
}

void RO_Config::UpdateCreateConfig(const json& data) {
  json existing_data;

  std::ifstream file_in("config.json");
  if (file_in.is_open()) {
    try {
      file_in >> existing_data;
    }
    catch (const json::parse_error& e) {
      // Handle parse error (e.g., empty file)
      std::cerr << "Error parsing JSON: " << e.what() << std::endl;
      existing_data = json::object();
    }
    file_in.close();
  }

  // Update existing data with the new data
  RecursiveUpdate(existing_data, data);

  // Write the updated data back to the file
  std::ofstream file_out("config.json");
  file_out << std::setw(4) << existing_data << std::endl;
  file_out.close();
}

void RO_Config::RecursiveUpdate(json& target, const json& source) {
  for (auto it = source.begin(); it != source.end(); ++it) {
    if (it.value().is_object() && target[it.key()].is_object()) {
      // If both target and source are objects, recursively update
      RecursiveUpdate(target[it.key()], it.value());
    }
    else {
      // Otherwise, update the value
      target[it.key()] = it.value();
    }
  }
}
