#pragma once
#include "structs.h"
#include "imgui.h"
#include "database.h"
#include "imgui_viewer.h"

struct ComboFromTable {
  std::unordered_map<int, std::string> values;
  std::string selection_value;
  int selected = 0;
  bool changed;
};

struct Filter : ComboFromTable {

  // Constructor to initialize with additional values
  Filter(const std::unordered_map<int, std::string>& additionalValues) {
    values.insert(std::make_pair(0, "None"));
    values.insert(additionalValues.begin(), additionalValues.end());
  }
};

class DeviceEditor {
public:
  DeviceEditor();
  DeviceEditor(DeviceDetailed& device);
public:
  static bool* show_device;
  DeviceDetailed device;
  void Render();
  void DeviceTypeEdit();
  void ListDeviceAttrs(const std::string label, std::vector<std::string>& attrs);
  bool ColorButton(const char* label, const float color);
  void ReloadDevice();
  static void Set(std::shared_ptr<DeviceEditor> device_editor);
  static std::shared_ptr<DeviceEditor> Get();
  static inline HintInputFieldsW_PopupStr color;
private:
  static inline bool changed = false;
  std::string new_attr;
  static std::shared_ptr<DeviceEditor> instance;
  static inline ComboFromTable types = { Database::GetSimpleTable("device_type") };
};

class DevicesView {
public:
  static void Render();
private:
  static void FilterCombo(const int& i, Filter& filter_data);
  static void OnFilterChange();
  static void DevicesTable();
private:
  static inline HintInputFieldsW_Popup model;
  static inline Filter brands = { Database::GetSimpleTable("brands") };
  static inline Filter types = { Database::GetSimpleTable("device_type") };
  static inline DevicesSort k_devices = Database::RetreiveDevices();
 
private:

  struct DeviceNode {
    std::string Name;
    std::string Type;
    std::string Brand;
    int             ChildIdx;
    int             ChildCount;
    static void DisplayNodeD(const DeviceNode* node, const std::vector<DeviceNode>& all_nodes, const int& i);
  };
  static void PopulateNodes();
  static std::vector<std::vector<DeviceNode>> device_nodes;
  static inline bool populated = false;
};



