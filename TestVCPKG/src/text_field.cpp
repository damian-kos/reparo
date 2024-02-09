#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "text_field.h"

TextField::TextField() {}

TextField::TextField(std::string label) : label(label){

}

void TextField::FieldProperties() {
  if (label.empty())
    label = "Text Field";
  if (ImGui::CollapsingHeader(label.c_str(), &closable_group)) {
    ImGui::DragFloat("Text pos x", &offset.x, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::DragFloat("Text pos y", &offset.y, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::InputTextMultiline("Text", text, IM_ARRAYSIZE(text));
    ImGui::DragFloat("Rect size", &size.x, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::DragFloat("Rect size ", &size.y, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::DragFloat("Font Size", &font_size, 1.0f, 2.0f, 0.0f, "%.0f");
  }
}

// SECTION LoadLogo
void Logo::FieldProperties() {
  std::cout << "Logo properties" << std::endl;
  if (ImGui::CollapsingHeader("Logo Image", &closable_group)) {
    ImGui::DragFloat("Text pos x", &offset.x, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::DragFloat("Text pos y", &offset.y, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::DragFloat("Rect size", &size.x, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::DragFloat("Rect size ", &size.y, 1.0f, 2.0f, 0.0f, "%.0f");
  }
}

ID3D11ShaderResourceView* LoadLogo::ReturnTexture() {
  if (my_texture != nullptr) {
    my_texture->Release(); // Release the previous texture
    my_texture = nullptr;
  }
  bool ret = LoadTextureFromFile("sloth.png", &my_texture, &my_image_width, &my_image_height);
  IM_ASSERT(ret);
  return my_texture;
}

ID3D11ShaderResourceView* LoadLogo::ShowLoadLogo() {
  if (ImGui::Button("Show image")) {
    ReturnTexture();
    return my_texture;
  }
  return nullptr;
}

bool LoadLogo::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
  // Load from disk into a raw RGBA buffer
  int image_width = 0;
  int image_height = 0;
  unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
  if (image_data == NULL)
    return false;

  // Create texture
  D3D11_TEXTURE2D_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Width = image_width;
  desc.Height = image_height;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = 0;

  ID3D11Texture2D* pTexture = NULL;
  D3D11_SUBRESOURCE_DATA subResource;
  subResource.pSysMem = image_data;
  subResource.SysMemPitch = desc.Width * 4;
  subResource.SysMemSlicePitch = 0;
  g_pd3dDeviceImages->CreateTexture2D(&desc, &subResource, &pTexture);

  // Create texture view
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  ZeroMemory(&srvDesc, sizeof(srvDesc));
  srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = desc.MipLevels;
  srvDesc.Texture2D.MostDetailedMip = 0;
  g_pd3dDeviceImages->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
  pTexture->Release();

  *out_width = 60;
  *out_height = 60;
  stbi_image_free(image_data);

  return true;
}
