#pragma once
#include "json.hpp"
#include <fstream>
#include <iostream>


typedef nlohmann::json json;

class RO_Config {
public:
  RO_Config();
public:
  static json GetConfig();
  static void UpdateCreateConfig(const json& data);

private:
  static void RecursiveUpdate(json& target, const json& source);
public:
  static json data;
};