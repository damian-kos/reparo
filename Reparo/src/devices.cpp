#include "devices.h"

std::shared_ptr<DeviceEditor> DeviceEditor::instance = nullptr;
bool* DeviceEditor::show_device = new bool(false);

DeviceEditor::DeviceEditor(DeviceDetailed& device) : device(device) {
  std::cout << device.model << std::endl;
}

void DeviceEditor::Render() {
  ImGui::Begin("Device Editor", show_device);
  // Model
  ImGui::BeginGroup();
  ImGui::Button(device.model.c_str());
  ImGui::EndGroup(); ImGui::SameLine();
  // Device type
  ImGui::BeginGroup();
  ImGui::Button(device.device_type.c_str());
  ImGui::EndGroup(); ImGui::SameLine();
  // Attributes
  ListDeviceAttrs("colors",device.colors);
  ListDeviceAttrs("aliases", device.aliases);
  ReloadDevice();
  ImGui::End();
}

void DeviceEditor::ListDeviceAttrs(const std::string label, std::vector<std::string>& attrs) {
  ImGui::PushID(label.c_str());
  ImGui::BeginGroup();
  for (auto& elem : attrs) {
    ImGui::Button(elem.c_str());
  }
  if (ColorButton("Add", 0.4f)) {
    ImGui::OpenPopup("Add");
    new_attr = "";
    //color.input.buffer = "";
  }
  if (ImGui::BeginPopup("Add")) {
    bool close_popup = false;
    if (label == "aliases") {
      if (ImGui::InputText("Insert new attribute", &new_attr, ImGuiInputTextFlags_EnterReturnsTrue)) {
        close_popup = true;
      }
    }
    else {
      ImGui::InputTextWithPopup("##Color3", "Color...", color, // Make it so enter returns true
        [&]() {  return  color.input.buffer.size() > 3; });
        new_attr = color.input.buffer;
    }
     
    if (ImGui::Button("Confirm")) {
      close_popup = true;
    }
    if (close_popup) {
      Database::UpdateDeviceDetails(device, label, new_attr);
      changed = true;
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
  ImGui::PopID();

  ImGui::EndGroup(); ImGui::SameLine();

  ImGui::BeginGroup();
  for (auto& elem : attrs) {
    ImGui::PushID(elem.c_str());
    if (ColorButton("x", 0.0f)) {
      Database::DeleteDeviceDetail(device, label, elem.c_str());
      changed = true;
    }
    ImGui::PopID();
  }
  ImGui::EndGroup(); ImGui::SameLine();
}

bool DeviceEditor::ColorButton(const char* label, const float color) {
  ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(color, 0.6f, 0.6f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(color, 0.7f, 0.7f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(color, 0.8f, 0.8f));
  bool pressed = ImGui::Button(label);
  ImGui::PopStyleColor(3);
  return pressed;
}

void DeviceEditor::Set(std::shared_ptr<DeviceEditor> device_editor) {
  instance = device_editor;
}

std::shared_ptr<DeviceEditor> DeviceEditor::Get() {
  return instance;
}

void DeviceEditor::ReloadDevice() {
  if (changed) {
    device = Database::RetreiveDevice(device.id);
    changed = false;
  }
}


std::vector<std::vector<DevicesView::DeviceNode>> DevicesView::device_nodes;

void DevicesView::Render() {
  ImGui::Begin("Devices");
  PopulateNodes();
  ImGui::InputTextWithPopup("##Model", "Model of device...", model,
    [&]() { return  strlen(model.input.buffer) > 3; }, NULL, NULL, NULL);
  FilterCombo(1, types);
  FilterCombo(2, brands);
  OnFilterChange();
  DevicesTable();
  
  ImGui::End();
}

void DevicesView::FilterCombo(const int& i, Filter& filter) {
  ImGui::PushID(i);
  if (ImGui::BeginCombo("Filter", filter.selection_value.c_str()))
  {
    for (auto& pair : filter.values) {
      const bool is_selected = (filter.selected == pair.first);
      if (ImGui::Selectable(filter.values[pair.first].c_str(), is_selected)) {
        filter.selected = pair.first;
        filter.selection_value = pair.second;
        filter.changed = true;
        std::cout << "Selected: " << filter.selected << std::endl;
      }
      if (is_selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  ImGui::PopID();
}


void DevicesView::OnFilterChange() {
  //if (brands.changed || types.changed ) {
  if(ImGui::Button("Search")){
    std::string search(model.input.buffer);
    k_devices.devices.clear();
    k_devices = Database::RetreiveDevices(types.selected, brands.selected, search);

    std::cout << k_devices.devices.size() << std::endl;
  }
}

void DevicesView::DevicesTable() {
  //const bool is_selected = false;
  static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
  static ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns;
  if (ImGui::BeginTable("3ways", 3, flags)) {
    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
    ImGui::TableSetupColumn("Brand");
    ImGui::TableSetupColumn("Type");
    ImGui::TableHeadersRow();
    //struct MyTreeNode
    //{
    //  std::string Name;
    //  std::string Type;
    //  std::string Brand;
    //  int             ChildIdx;
    //  int             ChildCount;
    //  static void DisplayNode(const MyTreeNode* node, const std::vector<MyTreeNode>& all_nodes, const int& i)
    //  {
    //    ImGui::TableNextRow();
    //    //ImGui::Selectable(node->Name.c_str(), is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap);
    //    ImGui::TableNextColumn();

    //    const bool is_folder = (node->ChildCount > 0);
    //    if (is_folder)
    //    {
    //      bool open = ImGui::TreeNodeEx(node->Name.c_str(), tree_node_flags);
    //      if (ImGui::BeginPopupContextItem()) {
    //        if (ImGui::Button("Edit device details")) {
    //          DeviceEditor::Set(std::make_shared<DeviceEditor>(k_devices.devices[i]));
    //          *DeviceEditor::show_device = true;
    //          std::cout << "Selected " << i << std::endl;
    //        }
    //        if (ImGui::Button("Delete device")) {
    //        }
    //        ImGui::EndPopup();
    //      }
    //      ImGui::TableNextColumn();
    //      if (node->ChildIdx == 1)
    //        ImGui::Text(node->Brand.c_str());
    //      else
    //        ImGui::Text("--");

    //      //ImGui::TextDisabled("--");
    //      ImGui::TableNextColumn();
    //      ImGui::TextUnformatted(node->Type.c_str());
    //      if (open)
    //      {
    //        for (int child_n = 0; child_n < node->ChildCount; child_n++)
    //          DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes, i);
    //        ImGui::TreePop();
    //      }
    //    }
    //    else
    //    {
    //      ImGui::TreeNodeEx(node->Name.c_str(), tree_node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
    //      ImGui::TableNextColumn();
    //      ImGui::TextDisabled("");
    //      ImGui::TableNextColumn();
    //      ImGui::TextUnformatted(node->Type.c_str());
    //    }
    //  }
    //};

    //if (k_devices.devices.empty()) {
    //}
    //else {
    //  for (int i = 0; i < k_devices.devices.size(); i++) {
    //    if (k_devices.devices[i].model != "") {
    //    std::vector<MyTreeNode> nodes;
    //    // Add root, colors, and aliases nodes
    //      nodes.push_back({ k_devices.devices[i].model.c_str(), k_devices.devices[i].device_type.c_str(), k_devices.devices[i].brand.c_str(), 1, 2 });
    //      nodes.push_back({ "Colors (" + std::to_string(k_devices.devices[i].colors.size()) + ")", "--", "--", 3, static_cast<int>(k_devices.devices[i].colors.size()) });
    //      nodes.push_back({ "Aliases (" + std::to_string(k_devices.devices[i].aliases.size()) + ")", "--", "--",  static_cast<int>(k_devices.devices[i].colors.size() + 3), static_cast<int>(k_devices.devices[i].aliases.size()) });
    //      for (auto& color : k_devices.devices[i].colors) {
    //        nodes.push_back({ color, "", "", -1, -1 });
    //      }
    //      for (auto& alias : k_devices.devices[i].aliases) {
    //        nodes.push_back({ alias, "", "", -1, -1 });
    //      }
    //      MyTreeNode::DisplayNode(&nodes[0], nodes, i);
    //    }
    //  }
    //}
    for (int i = 0; i < device_nodes.size(); i++)     {
      DeviceNode::DisplayNodeD(&device_nodes[i][0], device_nodes[0], i);
    }


    ImGui::EndTable();
  }
}

void DevicesView::PopulateNodes() {
  static bool populated = false;
  if (!populated) {
    for (int i = 0; i < k_devices.devices.size(); i++) {
      if (k_devices.devices[i].model != "") {
        std::vector<DeviceNode> nodes;
        nodes.push_back({ k_devices.devices[i].model.c_str(), k_devices.devices[i].device_type.c_str(), k_devices.devices[i].brand.c_str(), 1, 2 });
        nodes.push_back({ "Colors (" + std::to_string(k_devices.devices[i].colors.size()) + ")", "--", "--", 3, static_cast<int>(k_devices.devices[i].colors.size()) });
        nodes.push_back({ "Aliases (" + std::to_string(k_devices.devices[i].aliases.size()) + ")", "--", "--",  static_cast<int>(k_devices.devices[i].colors.size() + 3), static_cast<int>(k_devices.devices[i].aliases.size()) });
        for (auto& color : k_devices.devices[i].colors) {
          nodes.push_back({ color, "", "", -1, -1 });
        }
        for (auto& alias : k_devices.devices[i].aliases) {
          nodes.push_back({ alias, "", "", -1, -1 });
        }
        device_nodes.push_back(nodes);
        //DeviceNode::DisplayNodeD(&device_nodes[0], device_nodes, i);
      }
    }
    populated = true;
  }
}

void DevicesView::DeviceNode::DisplayNodeD(const DeviceNode* node, const std::vector<DeviceNode>& all_nodes, const int& i) {
  static ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns;
  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  const bool is_folder = (node->ChildCount > 0);
  if (is_folder)
  {
    bool open = ImGui::TreeNodeEx(node->Name.c_str(), tree_node_flags);
    if (ImGui::BeginPopupContextItem()) {
      if (ImGui::Button("Edit device details")) {
        DeviceEditor::Set(std::make_shared<DeviceEditor>(k_devices.devices[i+1]));
        *DeviceEditor::show_device = true;
        std::cout << "Selected " << i+1 << std::endl;
      }
      if (ImGui::Button("Delete device")) {
      }
      ImGui::EndPopup();
    }
    ImGui::TableNextColumn();
    if (node->ChildIdx == 1)
      ImGui::Text(node->Brand.c_str());
    else
      ImGui::Text("--");

    //ImGui::TextDisabled("--");
    ImGui::TableNextColumn();
    ImGui::TextUnformatted(node->Type.c_str());
    if (open)
    {
      for (int child_n = 0; child_n < node->ChildCount; child_n++)
        DisplayNodeD(&all_nodes[node->ChildIdx + child_n], all_nodes, i);
      ImGui::TreePop();
    }
  }
  else
  {
    ImGui::TreeNodeEx(node->Name.c_str(), tree_node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
    ImGui::TableNextColumn();
    ImGui::TextDisabled("");
    ImGui::TableNextColumn();
    ImGui::TextUnformatted(node->Type.c_str());
  }
}

