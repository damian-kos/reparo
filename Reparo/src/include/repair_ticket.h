#pragma once
#include "imgui.h"
#include <string>
#include <d3d11.h>
#include <imgui_internal.h>
#include <opencv2/opencv.hpp>
#include <opencv2/freetype.hpp>
#include <algorithm>

#include <string>
#include <vector>
#include "settings.h"
#include "imgui_viewer.h"
#include "IObserver.h"
#include "config.h"

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
  static bool MoveLogo(std::string source);
  static void RecalcSize(float& img_width, float& img_height);

public:
  static inline ID3D11ShaderResourceView* texture = nullptr;

};

class LoadImg {
public: 
  static void LoadLogo(const char* filename, Logo& logo);
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
  static void CreateA4(std::vector<TextField>& text_fields_vector, Logo* logo, int right_margin, Repair* repair = nullptr);
  //static void CreateA4(std::vector<TextField>& text_fields_vector, Logo* logo, float right_margin, Repair& repair);

  static void DrawTextFieldsOnImage(std::vector<TextField>& text_fields_vector, cv::Mat& image, int& right_margin, Repair* repair = nullptr);
  static int DrawWrappedText(cv::Mat& image, const std::string& text, cv::Point origin, int max_width, float font_scale, cv::Scalar color, int thickness);
  static void DrawRoundedRect(cv::Mat& image, cv::Point top_left, cv::Point bot_right, int corner_radius, const cv::Scalar& color, const cv::Scalar& color_fill, int thickness);
  static void DrawLogo(cv::Mat& image, Logo* logo);

private:
  static inline float print_dpi = TicketScales::print_dpi;
  static inline float scale =TicketScales::scale;
  static inline float dpi_scale = TicketScales::dpi_scale;


  //static inline float dpi_s = 1.5f;
  static inline int width = static_cast<int>(210 * print_dpi);
  static inline int height = static_cast<int>(297 * print_dpi);
  static inline cv::Ptr<cv::freetype::FreeType2> ft2 = cv::freetype::createFreeType2();
  static std::unordered_map<std::string, std::string> AssignRepairToLabels(Repair* repair);

private:
  static inline std::string lorem_ipsum = "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?";
};

// SECTION RepairTicket 
class RepairTicket : public IObserver {
public:
  void RepairTicketWin();
public:
  static inline bool show_window = false;
  static inline Logo logo;
  static inline TicketImage ticket_img;
  static inline ConfirmResult print = ConfirmResult::CONIFRM_IDLE;
  static inline bool run_modal = false;
private:
  static inline bool use_spacer;
  static inline float vec4f[4] = { 0.10f, 0.20f, 0.30f, 150.0f };
  std::vector<std::string> text_fields_labels{ "Repair number", "Date", "Phone", "Name", "Surname", "Email", "Model",
                                            "Category", "SN / IMEI", "Color", "Note for customer", "Price", "Terms & Conditions" };
  std::vector<TextField> text_fields_vector;
  static inline int text_fields_count = 0;

private:
  void DrawBlankPage(ImDrawList* draw_list, const ImRect& canvas_rect);
  void CreateTextFields();
  void ShowTextFieldsProperties();
  void TextFieldsOnCanvas(const ImRect& canvas_rect);
  void LoadImgAndProperties();
  void ShowLogoOnCanvas(const ImRect& canvas_rect);
  void SaveTemplateProperties();
  void CreateTemplate();
  void ShowTicket();
  static void PrintTicket();

public:
  void Update(const int& passed_int, Repair* repair) override;
  static void Modals();
};

