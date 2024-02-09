#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <string>
#include <iostream>
#include <memory>
#include <imgui_internal.h>

#include <vector>
#include "text_field.h"
#include "config.h"


class Images {
public:
  void CreateImage();
  void TextFieldsForTemplate();
  void ShowImage();
  bool InvisibleButtonEx(const char* str_id, ImGuiButtonFlags flags, TextField& text_field, ImRect& canvas);
  bool InvisibleButtonEx(const char* str_id, ImGuiButtonFlags flags, ImRect& canvas, ImVec2& offset, Logo* logo);
  std::vector<TextField> text_fields_vector;

  static void SetFonts();
  void SaveTemplate();

private:
  static inline ImFont* font = nullptr;
  static inline Logo logo;
  static inline int text_fields_count = 0;
  std::vector<std::string> text_fields_labels{"Repair number:", "Phone", "Name", "Surname", "Email", "Model", "Category", "Color", "Note for customer", "Price", "Terms & Conditions"};

};