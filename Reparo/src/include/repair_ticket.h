#pragma once
#include "imgui.h"
#include <string>
#include <d3d11.h>
#include <imgui_internal.h>
#include <string>
#include <vector>
#include "settings.h"
#include "imgui_viewer.h"


class ElementProperty {
public:
  ImVec2 size = { 50, 50 };
  ImVec2 offset = { 0 , 0 };
  bool closable_group = true;
};

// SECTION TextField
class TextField : public ElementProperty {
public:
  TextField();
  TextField(std::string label);

public:
  std::string label = "";
  char text[512] = "";
  void SetProperties();
  float font_size = 18.0f;
};

// SECTION LoadLogo load
class Logo : public ElementProperty {
public:
  void SetProperties();
public:
  ID3D11ShaderResourceView* texture = nullptr;
};

class LoadLogo {

public: 
  static ID3D11ShaderResourceView* ShowLoadLogo();
public:
  static ID3D11ShaderResourceView* ReturnTexture();
  static bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
public:
  static inline ID3D11Device* g_pd3dDeviceImages;
  static inline ID3D11ShaderResourceView* texture = nullptr;
  static inline int my_image_width = static_cast<int>(210);
  static inline int my_image_height = static_cast<int>(297);
};

class RepairTicket {
public:
  void RepairTicketWin();
public:
  static inline bool show_window = false;
private:
  void DrawBlankPage(ImDrawList* draw_list, const ImRect& canvas_rect);
  void CreateTextFields();
  void ShowTextFieldsProperties();
  void TextFieldsOnCanvas(const ImRect& canvas_rect);
  void LoadLogoAndProperties();
  void ShowLogoOnCanvas(const ImRect& canvas_rect);
  void SaveTemplateProperties();
private:
  std::vector<std::string> text_fields_labels{ "Repair number", "Date", "Phone", "Name", "Surname", "Email", "Model", 
                                              "Category", "SN / IMEI", "Color", "Note for customer", "Price", "Terms & Conditions" };
  std::vector<TextField> text_fields_vector;
  static inline int text_fields_count = 0;
  static inline Logo logo;
};