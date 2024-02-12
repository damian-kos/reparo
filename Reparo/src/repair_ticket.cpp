#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "repair_ticket.h"

void RepairTicket::RepairTicketWin() {
  ImGui::Begin("Template Editor");

    if (ImGui::BeginTable("Table", 2, ImGuiTableFlags_Resizable)) {
      ImGui::TableSetupColumn("Column 2", ImGuiTableColumnFlags_WidthFixed, 350.0f); // Fixed width
      ImGui::TableSetupColumn("Column 1"); // Default settings
      ImGui::TableNextRow();
      ImGui::TableNextColumn();                                                // First column

      CreateTextFields();
      ShowTextFieldsProperties();
      LoadLogoAndProperties();
      SaveTemplateProperties();
      ImGui::TableNextColumn();                                                // Second column 
      // CreateImage() - opencv
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
      ImVec2 canvas_p1 = ImVec2(canvas_p0.x + (210 * 2.5), canvas_p0.y + (297 * 2.5));
      ImRect canvas_rect = ImRect(canvas_p0, canvas_p1);
      DrawBlankPage(draw_list, canvas_rect);
      TextFieldsOnCanvas(canvas_rect);
      ShowLogoOnCanvas(canvas_rect);



    ImGui::EndTable();
    }

  ImGui::End();
}

void RepairTicket::DrawBlankPage(ImDrawList* draw_list, const ImRect& canvas) {
  draw_list->AddRectFilled(canvas.Min, canvas.Max, IM_COL32(255, 255, 255, 255));
  draw_list->AddRect(canvas.Min, canvas.Max, IM_COL32(255, 255, 255, 255));
}

void RepairTicket::CreateTextFields() {
  if (ImGui::Button("Load TextFields")) {
    //json data = RO_Settings::GetConfig("")`
    for (auto& label : text_fields_labels) {
      TextField text_field(label);
      strcpy_s(text_field.text, label.c_str());
      text_field.offset.x = RO_Cfg::getValue("text_field." + label + ".offfset.x", 100.0f, "template.json");
      text_field.offset.y = RO_Cfg::getValue("text_field." + label + ".offfset.y", 100.0f, "template.json");
      text_field.size.x = RO_Cfg::getValue("text_field." + label + ".size.x", 50.0f, "template.json");
      text_field.size.y = RO_Cfg::getValue("text_field." + label + ".size.y", 50.0f, "template.json");
      text_field.font_size = RO_Cfg::getValue("text_field." + label + ".font_size", 14.0f, "template.json");

      text_fields_vector.push_back(text_field);
      text_fields_count += 1;
    }
  }
}

void RepairTicket::ShowTextFieldsProperties() {
  for (int i = 0; i < text_fields_count; i++) {
    ImGui::PushID(i);
    text_fields_vector[i].SetProperties();
    ImGui::PopID();
    if (!text_fields_vector[i].closable_group) {
      if (i >= 0 && i < text_fields_vector.size()) {
        text_fields_vector.erase(text_fields_vector.begin() + i);
      }
      text_fields_count -= 1;
    }
  }
}

void RepairTicket::TextFieldsOnCanvas(const ImRect& canvas_rect) {
  for (int i = 0; i < text_fields_count; i++) {
    /*if (use_spacer) {
      text_fields_vector[i].offset.y = vec4f[3] + (vec4f[0] * i);
      text_fields_vector[i].size.x = 50 + (vec4f[1]);
      text_fields_vector[i].size.y = 50 + (vec4f[2]);
    }*/
    ImGui::PushID(i);
    ImGui::InvisibleButtonEx("canvas1 %d", ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight, text_fields_vector[i], canvas_rect);
    ImGui::PopID();
  }
}

void RepairTicket::LoadLogoAndProperties() {
  if (logo.texture == nullptr) {
    logo.texture = LoadLogo::ShowLoadLogo();
  }
  else {
    static bool logo_init = false;
    if (!logo_init) {
      logo.offset.x = RO_Cfg::getValue("logo.offfset.x", 100.0f, "template.json");
      logo.offset.y = RO_Cfg::getValue("logo.offfset.y", 100.0f, "template.json");
      logo.size.x = RO_Cfg::getValue("logo.size.x", 100.0f, "template.json");
      logo.size.y = RO_Cfg::getValue("logo.size.y", 100.0f, "template.json");
      logo_init = true;
    }
    logo.SetProperties();
  }
}

void RepairTicket::ShowLogoOnCanvas(const ImRect& canvas_rect) {
  ImGui::InvisibleButtonEx("canvas2 %d", ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight, canvas_rect, logo.offset, &logo);
}

void RepairTicket::SaveTemplateProperties() {
    if (ImGui::Button("Save template")) {
      for (auto& field : text_fields_vector) {
        std::string path = "text_field." + field.label;
        RO_Cfg::UpdateCreateConfig(path + ".offfset.x", field.offset.x, "template.json");
        RO_Cfg::UpdateCreateConfig(path + ".offfset.y", field.offset.y, "template.json");
        RO_Cfg::UpdateCreateConfig(path + ".size.x", field.size.x, "template.json");
        RO_Cfg::UpdateCreateConfig(path + ".size.y", field.size.y, "template.json");
        RO_Cfg::UpdateCreateConfig(path + ".font_size", field.font_size, "template.json");
      }
      RO_Cfg::UpdateCreateConfig("logo.offfset.x", logo.offset.x, "template.json");
      RO_Cfg::UpdateCreateConfig("logo.offfset.y", logo.offset.y, "template.json");
      RO_Cfg::UpdateCreateConfig("logo.size.x", logo.size.x, "template.json");
      RO_Cfg::UpdateCreateConfig("logo.size.y", logo.size.y, "template.json");
    }
}

TextField::TextField() {}

TextField::TextField(std::string label) : label(label) {

}

void TextField::SetProperties() {
  if (label.empty())
    label = "Text Field";
  if (ImGui::CollapsingHeader(label.c_str(), &closable_group)) {
    ImGui::DragFloat("Position on X axis", &offset.x, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::DragFloat("Position on Y axis", &offset.y, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::InputTextMultiline("Text", text, IM_ARRAYSIZE(text));
    ImGui::DragFloat("Box X size", &size.x, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::DragFloat("Box Y size ", &size.y, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::DragFloat("Font size", &font_size, 1.0f, 2.0f, 0.0f, "%.0f");
  }
}

// SECTION LoadLogo
void Logo::SetProperties() {
  if (ImGui::CollapsingHeader("Logo Image", &closable_group)) {
    ImGui::DragFloat("Image position on X axis", &offset.x, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::DragFloat("Image position on Y axis", &offset.y, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::DragFloat("Image X size", &size.x, 1.0f, 2.0f, 0.0f, "%.0f");
    ImGui::DragFloat("Image Y size ", &size.y, 1.0f, 2.0f, 0.0f, "%.0f");
  }
}

ID3D11ShaderResourceView* LoadLogo::ShowLoadLogo() {
  if (ImGui::Button("Show logo")) {
    ReturnTexture();
    return texture;
  }
  return nullptr;
}

ID3D11ShaderResourceView* LoadLogo::ReturnTexture() {
  if (texture != nullptr) {
    texture->Release(); // Release the previous texture
    texture = nullptr;
  }
  bool ret = LoadTextureFromFile("logo.png", &texture, &my_image_width, &my_image_height);
  IM_ASSERT(ret);
  return texture;
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

  ID3D11Texture2D* p_texture = NULL;
  D3D11_SUBRESOURCE_DATA sub_resource;
  sub_resource.pSysMem = image_data;
  sub_resource.SysMemPitch = desc.Width * 4;
  sub_resource.SysMemSlicePitch = 0;
  g_pd3dDeviceImages->CreateTexture2D(&desc, &sub_resource, &p_texture);

  // Create texture view
  D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
  ZeroMemory(&srv_desc, sizeof(srv_desc));
  srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srv_desc.Texture2D.MipLevels = desc.MipLevels;
  srv_desc.Texture2D.MostDetailedMip = 0;
  g_pd3dDeviceImages->CreateShaderResourceView(p_texture, &srv_desc, out_srv);
  p_texture->Release();

  *out_width = 150;
  *out_height = 150;
  stbi_image_free(image_data);

  return true;
}

