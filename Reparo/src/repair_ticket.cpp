#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "repair_ticket.h"

// SECTION TextField
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
  ImGui::SameLine();
  ImGui::Checkbox("Checkbox", &editable);
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

// SECTION CreateImage

void CreateImage::CreateA4(std::vector<TextField>& text_fields_vector, Logo* logo, float right_margin) {
  // Convert A4 size from inches to pixels (corrected conversion using PPI)

  cv::Mat image(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

  // Create a FreeType2 instance and load font we will be using
  //ft2 = cv::freetype::createFreeType2();
  std::string fontPath = "vendor/imgui/msc/fonts/assistant.ttf";
  ft2->loadFontData(fontPath, 0);
  DrawTextFields(text_fields_vector, image, right_margin);

  ft2->putText(image, "Reference text", cv::Point(30, 30), 32, cv::Scalar (0, 0, 0), -1, cv::LINE_AA, true);
  cv::putText(image, "Reference text", cv::Point(30,120), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 0), 1, cv::LINE_4);
  
  DrawLogo(image, logo);

  cv::imwrite("a4_hello_world.jpg", image); // Save the image
  cv::imshow("A4 Image with Custom Font", image); // Display the image
  cv::waitKey(0); // Wait for a key press to close the image window
}

void CreateImage::DrawTextFields(std::vector<TextField>& text_fields_vector, cv::Mat& image, float& right_margin) {
  static bool toggle = false;
  for (const auto& field : text_fields_vector) {

    float offset_x = field.offset.x * dpi_s;
    float offset_y = field.offset.y * dpi_s;

    cv::Scalar rect_color(0, 0, 0); // Black border
    int thickness = 1; // Pixel
    int corner_radius = 5; // Radius of the rounded corners
    cv::Scalar color_fill = toggle ? cv::Scalar(255, 235, 0, 128) : cv::Scalar(255, 175, 0, 128);

    cv::Point left_cell;
    if (field.label == "Terms & Conditions") {
      left_cell = cv::Point(width - 25, (offset_y)+(field.size.y * dpi_s));
    }
    else {
      left_cell = cv::Point((offset_x)+(field.size.x * dpi_s), (offset_y)+(field.size.y * dpi_s));
    }
    DrawRoundedRect(image, cv::Point((offset_x)-field.font_size / 3, (offset_y)-field.font_size / 4),
      left_cell,
      corner_radius, rect_color, color_fill, thickness);
    cv::Scalar color_fill1(255, 255, 255, 128);

    cv::Point right_cell;
    if (field.label != "Terms & Conditions") {
      if (field.label == "Repair number:")
        right_cell = cv::Point((offset_x)+(field.size.x * dpi_s) * 2, (offset_y)+(field.size.y * dpi_s));
      else {
        right_cell = cv::Point(width - right_margin, (field.offset.y + field.size.y) * dpi_s);
      }
      DrawRoundedRect(image, cv::Point((field.offset.x * dpi_s) + (field.size.x * dpi_s) + 1, (offset_y)-field.font_size / 4),
        right_cell,
        corner_radius, rect_color, color_fill1, thickness);
    }

    // Draw the text on the A4 image using custom font, handling new lines
    DrawWrappedText(image, field.text, cv::Point(offset_x, offset_y), field.size.x*dpi_s, field.font_size * dpi_s, cv::Scalar(0, 0, 0));


    std::cout << "Font size on image creation: " << field.font_size << std::endl;
    toggle = !toggle;
  }
}

void CreateImage::DrawWrappedText(cv::Mat& image, const std::string& text, cv::Point origin, int max_width, float font_scale, cv::Scalar color) {
  int base_line = 0;
  // Estimate height of a single line of text
  cv::Size text_size = ft2->getTextSize("W", static_cast<int>(font_scale), -1, &base_line);
  int y = origin.y;

  std::istringstream words(text);
  std::string line;
  std::string word;

  while (words >> word) {
    std::string mew_line = line + word + " ";
    cv::Size lineWidth = ft2->getTextSize(mew_line, static_cast<int>(font_scale), -1, &base_line);

    if (lineWidth.width > max_width && !line.empty()) {
      ft2->putText(image, line, cv::Point(origin.x, y), static_cast<int>(font_scale), color, -1, cv::LINE_AA, true);
      line = word + " ";
      y += text_size.height + base_line;
    }
    else {
      line = mew_line;
    }
  }
  if (!line.empty()) {
    ft2->putText(image, line, cv::Point(origin.x, y), static_cast<int>(font_scale), color, -1, cv::LINE_AA, true);
  }
}

void CreateImage::DrawRoundedRect(cv::Mat& image, cv::Point top_left, cv::Point bot_right, int corner_radius, const cv::Scalar& color, const cv::Scalar& color_fill, int thickness) {
  // Ensure thickness is positive or filled
  thickness = thickness > 0 ? thickness : -1;

  // Calculate corner points
  int x1 = top_left.x;
  int y1 = top_left.y;
  int x2 = bot_right.x;
  int y2 = bot_right.y;

  // Draw the four corners: top left, top right, bottom right, bottom left
  cv::ellipse(image, cv::Point(x1 + corner_radius, y1 + corner_radius), cv::Size(corner_radius, corner_radius), 180.0, 0, 90, color, thickness);
  cv::ellipse(image, cv::Point(x2 - corner_radius, y1 + corner_radius), cv::Size(corner_radius, corner_radius), 270.0, 0, 90, color, thickness);
  cv::ellipse(image, cv::Point(x2 - corner_radius, y2 - corner_radius), cv::Size(corner_radius, corner_radius), 0.0, 0, 90, color, thickness);
  cv::ellipse(image, cv::Point(x1 + corner_radius, y2 - corner_radius), cv::Size(corner_radius, corner_radius), 90.0, 0, 90, color, thickness);

  // Draw the four sides: top, right, bottom, left
  cv::line(image, cv::Point(x1 + corner_radius, y1), cv::Point(x2 - corner_radius, y1), color, thickness);
  cv::line(image, cv::Point(x2, y1 + corner_radius), cv::Point(x2, y2 - corner_radius), color, thickness);
  cv::line(image, cv::Point(x1 + corner_radius, y2), cv::Point(x2 - corner_radius, y2), color, thickness);
  cv::line(image, cv::Point(x1, y1 + corner_radius), cv::Point(x1, y2 - corner_radius), color, thickness);
  // choose arbitrary starting point for fill => Top left plus 10,10
  cv::Point fill_from(top_left.x + 10, top_left.y + 10);
  // You may want to use `lineColor` instead of `fillColor`
  std::cout << "Channels: " << image.channels() << std::endl;
  cv::floodFill(image, fill_from, color_fill);
}

void CreateImage::DrawLogo(cv::Mat& image, Logo* logo) {
  cv::Mat logo_img = cv::imread("logo.png", cv::IMREAD_UNCHANGED); // Load the extra image with alpha channel
  if (!logo_img.empty()) {
    // Resize the extra image to match logo size
    cv::Size new_size(logo->size.x * dpi_s, logo->size.y * dpi_s); // Assuming logo->size is defined and contains the target size
    cv::resize(logo_img, logo_img, new_size);

    // Define the region of interest (ROI) on the image
    cv::Point offset(logo->offset.x * dpi_s, logo->offset.y * dpi_s); // Assuming logo->offset is defined and contains the target position
    if (offset.x + new_size.width <= image.cols && offset.y + new_size.height <= image.rows) {
      cv::Rect roi(offset, new_size);
      cv::Mat destination_roi = image(roi);

      // Split the overlay image into channels
      std::vector<cv::Mat> channels(4);
      cv::split(logo_img, channels);

      // Channels[3] is the alpha channel
      cv::Mat alpha = channels[3];
      cv::Mat color_channels[3] = { channels[0], channels[1], channels[2] };

      // Merge the color channels back into an image
      cv::Mat color_image;
      cv::merge(color_channels, 3, color_image);

      // Use the alpha channel as a mask to combine the images
      cv::Mat masked_part;
      color_image.copyTo(masked_part, alpha); // Copy where alpha is not 0
      destination_roi.setTo(cv::Scalar(0, 0, 0, 0), alpha); // Clear the area in destination
      masked_part.copyTo(destination_roi, alpha); // Copy using alpha as mask
    }
    else {
      std::cerr << "The ROI is outside the bounds of the base image." << std::endl;
    }
  }
}

// SECTION RepairTicket
void RepairTicket::RepairTicketWin() {
  ImGui::Begin("Template Editor");

    if (ImGui::BeginTable("Table", 2, ImGuiTableFlags_Resizable)) {
      ImGui::TableSetupColumn("Column 2", ImGuiTableColumnFlags_WidthFixed, 350.0f); // Fixed width
      ImGui::TableSetupColumn("Column 1"); // Default settings
      ImGui::TableNextRow();
      ImGui::TableNextColumn();                                                // First column
      CreateTemplate();
      ImGui::DragFloat4("Spacer", vec4f); ImGui::SameLine();
      ImGui::Checkbox("Use spacer", &use_spacer);
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
    if (use_spacer && text_fields_vector[i].editable) {
      text_fields_vector[i].offset.x = 50.0f;
      text_fields_vector[i].offset.y = vec4f[3] + (vec4f[0] * i);
      text_fields_vector[i].size.x = 50 + (vec4f[1]);
      text_fields_vector[i].size.y = 50 + (vec4f[2]);
    }
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

void RepairTicket::CreateTemplate() {
  if (ImGui::Button("A4 IMAGE")) {
    CreateImage::CreateA4(text_fields_vector,  &logo, 25);
  }
}

