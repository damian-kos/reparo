#include "config.h"

 json RO_Cfg::data = RO_Cfg::GetConfig();

RO_Cfg::RO_Cfg(){
  printf("RO_Cfg created\n");
}

json RO_Cfg::GetConfig() {
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

json RO_Cfg::GetConfig(const std::string& filename) {
  json data;

  // Try to read existing data from the file
  std::ifstream file_in(filename);
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

void RO_Cfg::UpdateCreateConfig(const json& data) {
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

void RO_Cfg::RecursiveUpdate(json& target, const json& source) {
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

template <typename T>
void RO_Cfg::UpdateCreateConfig(const std::string& keyPath, const T& value) {
  json existing_data;

  std::ifstream file_in("config.json");
  if (file_in.is_open()) {
    try {
      file_in >> existing_data;
    }
    catch (const json::parse_error& e) {
      std::cerr << "Error parsing JSON: " << e.what() << std::endl;
      existing_data = json::object();
    }
  }
  else {
    std::cerr << "Error opening config file for reading." << std::endl;
    return;
  }

  file_in.close();

  // Split the key path into nested keys
  std::istringstream iss(keyPath);
  std::string item;
  json* currentNode = &existing_data;

  while (std::getline(iss, item, '.')) {
    currentNode = &(*currentNode)[item];
  }

  *currentNode = value;

  std::ofstream file_out("config.json");
  if (file_out.is_open()) {
    file_out << std::setw(4) << existing_data << std::endl;
  }
  else {
    std::cerr << "Error opening config file for writing." << std::endl;
  }

  file_out.close();
}

// Test config ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ BEGIN
template <typename T>
T RO_Cfg::getValue(const std::string& keyPath, const T& defaultValue, const std::string& filename) {
  std::cout << "Get value with filename is running." << std::endl;
  json loaded_data = RO_Cfg::GetConfig(filename);
  try {
    std::cout << "Loaded from specific json data " << std::endl;
    return getJsonValue<T>(loaded_data, keyPath);
  }
  catch (const std::exception& e) {
    // Print out the error message to std::cerr
    std::cerr << "Error retrieving value for key '" << keyPath << "': " << e.what() << "|" << std::endl;
    // Return the default value as a fallback
    return defaultValue;
  }
}

template <typename T>
void RO_Cfg::UpdateCreateConfig(const std::string& keyPath, const T& value, const std::string& filename) {
  //json loaded_data = RO_Cfg::GetConfig(filename);
  json existing_data;
  std::ifstream file_in(filename);
  if (file_in.is_open()) {
    try {
      file_in >> existing_data;
    }
    catch (const json::parse_error& e) {
      std::cerr << "Error parsing JSON: " << e.what() << std::endl;
      existing_data = json::object();
    }
  }
  else {
    std::cerr << "Error opening config file for reading." << std::endl;
    return;
  }

  file_in.close();

  // Split the key path into nested keys
  std::istringstream iss(keyPath);
  std::string item;
  json* currentNode = &existing_data;

  while (std::getline(iss, item, '.')) {
    currentNode = &(*currentNode)[item];
  }

  *currentNode = value;

  std::ofstream file_out(filename);
  if (file_out.is_open()) {
    file_out << std::setw(4) << existing_data << std::endl;
  }
  else {
    std::cerr << "Error opening config file for writing." << std::endl;
  }

  file_out.close();
}


// Test config ++++++++++++++++++++++++++++++++++++++++++++++++++++++++ END

template <typename T>
T RO_Cfg::getValue(const std::string& keyPath, const T& defaultValue) {
  try {
    return getJsonValue<T>(data, keyPath);
  }
  catch (const std::exception& e) {
    // Print out the error message to std::cerr
    std::cerr << "Error retrieving value for key '" << keyPath << "': " << e.what()  << "|" << std::endl;
    // Return the default value as a fallback
    return defaultValue;
  }
}

template <typename T>
T RO_Cfg::getJsonValue(const json& jsonData, const std::string& key) {
  std::istringstream iss(key);
  std::string item;
  json currentNode = jsonData;

  while (std::getline(iss, item, '.')) {
    if (!currentNode.contains(item)) {
      throw std::runtime_error("Key not found: " + item);
    }
    currentNode = currentNode[item];
  }

  return currentNode.get<T>();
}

// Explicit template instantiations
template bool RO_Cfg::getValue<bool>(const std::string&, const bool&);
template bool RO_Cfg::getJsonValue<bool>(const json&, const std::string&);

template int RO_Cfg::getValue<int>(const std::string&, const int&);
template int RO_Cfg::getJsonValue<int>(const json&, const std::string&);

template void RO_Cfg::UpdateCreateConfig<bool>(const std::string&, const bool&);


// // Explicit template instantiations with file_path
template bool RO_Cfg::getValue<bool>(const std::string&, const bool&, const std::string&);
template float RO_Cfg::getValue<float>(const std::string&, const float&, const std::string&);

template void RO_Cfg::UpdateCreateConfig<bool>(const std::string&, const bool&, const std::string&);
template void RO_Cfg::UpdateCreateConfig<float>(const std::string&, const float&, const std::string&);




