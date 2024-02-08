#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <string>
#include <iostream>
#include <imgui_internal.h>

class DrawElement {
public:
  ImVec2 size = { 50, 50 };
  ImVec2 offset = { 0 , 0 };
  bool closable_group = true;
};

// SECTION TextField

class TextField : public DrawElement {
public:
  char text[256] = "";
  void FieldProperties();
  float font_size = 18.0f;
};

// SECTION LoadLogo load

class LoadLogo {

public: // IMGUI
  static ID3D11ShaderResourceView* ShowLoadLogo();
public:
  static ID3D11ShaderResourceView* ReturnTexture();
  static bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
public:
  static inline ID3D11Device* g_pd3dDeviceImages;
  static inline ID3D11ShaderResourceView* my_texture = nullptr;
  static inline int my_image_width = static_cast<int>(210);
  static inline  int my_image_height = static_cast<int>(297);
};

class Logo : public DrawElement{
public:
  void FieldProperties();
public:
  ID3D11ShaderResourceView* my_texture = nullptr;
};