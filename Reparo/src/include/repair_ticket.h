#pragma once
#include "imgui.h"
#include <string>
#include <d3d11.h>
#include <imgui_internal.h>
#include <opencv2/opencv.hpp>
#include <opencv2/freetype.hpp>

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
  bool editable = true;
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

// SECTION CreateImage
class CreateImage {
public:
  static void CreateA4(std::vector<TextField>& text_fields_vector, Logo* logo, float right_margin);
  static void DrawTextFields(std::vector<TextField>& text_fields_vector, cv::Mat& image, float& right_margin);
  static void DrawWrappedText(cv::Mat& image, const std::string& text, cv::Point origin, int max_width, float font_scale, cv::Scalar color);
  static void DrawRoundedRect(cv::Mat& image, cv::Point top_left, cv::Point bot_right, int corner_radius, const cv::Scalar& color, const cv::Scalar& color_fill, int thickness);
  static void DrawLogo(cv::Mat& image, Logo* logo);
  static inline int dpi =5;
  static inline float dpi_s = 1.5f;
private:
  static inline int width = static_cast<int>(210 * dpi);
  static inline int height = static_cast<int>(297 * dpi);
  static inline cv::Ptr<cv::freetype::FreeType2> ft2 = cv::freetype::createFreeType2();
};

// SECTION RepairTicket 
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
  void CreateTemplate();
private:
  std::vector<std::string> text_fields_labels{ "Repair number", "Date", "Phone", "Name", "Surname", "Email", "Model", 
                                              "Category", "SN / IMEI", "Color", "Note for customer", "Price", "Terms & Conditions" };
  std::vector<TextField> text_fields_vector;
  static inline int text_fields_count = 0;
  static inline Logo logo;
private:
  static inline bool use_spacer;
  static inline float vec4f[4] = { 0.10f, 0.20f, 0.30f, 150.0f };
};

