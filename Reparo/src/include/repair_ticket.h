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
#include "IObserver.h"

class Logo;

class ElementProperty {
public:
  ElementProperty();
  ElementProperty(const ImVec2& size, const ImVec2& offset);
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
  TextField(const std::string& label, const ImVec2& size, const ImVec2& offset, const float& font_size);

public:
  std::string label = "";
  char text[512] = "";
  void SetProperties();
  float font_size = 18.0f;
  bool editable = true;
};

// SECTION TicketImage

class TicketImage {
public:
  static inline ID3D11ShaderResourceView* texture = nullptr;
};


// SECTION LoadImg load
class Logo : public ElementProperty {
public:
  Logo();
  Logo(const ImVec2& size, const ImVec2& offset);
public:
  void SetProperties();
public:
  static inline ID3D11ShaderResourceView* texture = nullptr;
};

class LoadImg {
public: 
  static void ShowLoadImg(const char* filename, ID3D11ShaderResourceView** texture);
public:
  static void ReturnTexture(const char* filename, ID3D11ShaderResourceView** texture);
  static bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
  static inline ID3D11ShaderResourceView* i_texture = nullptr;

public:
  static inline ID3D11Device* g_pd3dDeviceImages;
  //static inline ID3D11ShaderResourceView* texture = nullptr;
  static inline int my_image_width = static_cast<int>(210);
  static inline int my_image_height = static_cast<int>(297);
};

// SECTION CreateImage
class CreateImage  {
public:
  static void CreateA4(std::vector<TextField>& text_fields_vector, Logo* logo, float right_margin, Repair* repair = nullptr);
  //static void CreateA4(std::vector<TextField>& text_fields_vector, Logo* logo, float right_margin, Repair& repair);

  static void DrawTextFieldsOnImage(std::vector<TextField>& text_fields_vector, cv::Mat& image, float& right_margin, Repair* repair = nullptr);
  static void DrawWrappedText(cv::Mat& image, const std::string& text, cv::Point origin, int max_width, float font_scale, cv::Scalar color, int thickness);
  static void DrawRoundedRect(cv::Mat& image, cv::Point top_left, cv::Point bot_right, int corner_radius, const cv::Scalar& color, const cv::Scalar& color_fill, int thickness);
  static void DrawLogo(cv::Mat& image, Logo* logo);

private:
  static inline int dpi =5;
  static inline float dpi_s = 1.5f;
  static inline int width = static_cast<int>(210 * dpi);
  static inline int height = static_cast<int>(297 * dpi);
  static inline cv::Ptr<cv::freetype::FreeType2> ft2 = cv::freetype::createFreeType2();
  static std::unordered_map<std::string, std::string> AssignRepairToLabels(Repair* repair);

};

// SECTION RepairTicket 
class RepairTicket : public IObserver {
public:
  void RepairTicketWin();
public:
  static inline bool show_window = false;
private:
  void DrawBlankPage(ImDrawList* draw_list, const ImRect& canvas_rect);
  void CreateTextFields();
  void ShowTextFieldsProperties();
  void TextFieldsOnCanvas(const ImRect& canvas_rect);
  void LoadImgAndProperties();
  void ShowLogoOnCanvas(const ImRect& canvas_rect);
  void SaveTemplateProperties();
  void CreateTemplate();
  void ShowTemplate();

private:
  std::vector<std::string> text_fields_labels{ "Repair number", "Date", "Phone", "Name", "Surname", "Email", "Model", 
                                              "Category", "SN / IMEI", "Color", "Note for customer", "Price", "Terms & Conditions" };
  std::vector<TextField> text_fields_vector;
  static inline int text_fields_count = 0;
public:
  static inline Logo logo;
  static inline TicketImage ticket_img;
private:
  static inline bool use_spacer;
  static inline float vec4f[4] = { 0.10f, 0.20f, 0.30f, 150.0f };
private:

public:
  static inline ConfirmResult print = ConfirmResult::CONIFRM_IDLE;
  static inline bool run_modal = false;
  void Update(const int& passed_int, Repair* repair) override;

};

