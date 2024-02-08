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


class Images {
public:
  void CreateImage();
  void ShowImage();
  bool InvisibleButtonEx(const char* str_id, ImGuiButtonFlags flags, TextField& text_field, ImRect& canvas);
  bool InvisibleButtonEx(const char* str_id, ImGuiButtonFlags flags, const ImVec2& size_arg, ImRect& canvas, ImTextureID texture_id, ImVec2& offset, Logo* logo);
  std::vector<TextField> text_fields_vector;

  static void SetFonts();

private:
  static inline ImFont* font = nullptr;
  static inline Logo logo;
};