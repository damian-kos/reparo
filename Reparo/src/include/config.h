#pragma once
#include "json.hpp"
#include <fstream>
#include <iostream>


typedef nlohmann::json json;

class RO_Cfg {
public:
  RO_Cfg();
public:
  static json GetConfig();
  static json GetConfig(const std::string& filename);
  static void UpdateCreateConfig(const json& data);

private:
  static void RecursiveUpdate(json& target, const json& source);

public:
  static json data;


public:
  template<typename T>
  static void UpdateCreateConfig(const std::string& keyPath, const T& value);
  template <typename T>
  static T getValue(const std::string& keyPath, const T& defaultValue);

  template <typename T>
  static T getValue(const std::string& keyPath, const T& defaultValue, const std::string& filename);

  template<typename T>
  static void UpdateCreateConfig(const std::string& keyPath, const T& value, const std::string& filename);

private:
  template <typename T>
  static T getJsonValue(const json& jsonData, const std::string& key);





};
