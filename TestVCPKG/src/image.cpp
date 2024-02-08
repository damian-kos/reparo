#define IMGUI_DEFINE_MATH_OPERATORS
#include "image.h"
#include "blank_o_cv.h"

void Images::CreateImage() {
  if (ImGui::Button("A4 IMAGE")) {
    ImageEditor::CreateA4(text_fields_vector, &logo);
  }
}

void Images::ShowImage() {
  //static Logo logo;
  if (ImGui::BeginTable("Table", 2, ImGuiTableFlags_Resizable)) {
    ImGui::TableSetupColumn("Column 2", ImGuiTableColumnFlags_WidthFixed, 350.0f); // Fixed width
    ImGui::TableSetupColumn("Column 1"); // Default settings

    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    CreateImage();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    static int text_fields_count;
    if (ImGui::Button("Add text field")) {
      text_fields_count += 1;
      TextField field;
      text_fields_vector.push_back(field);
    }
    if (ImGui::Button("Remove text field")) {
      text_fields_vector.pop_back(); // Removes the last element
      text_fields_count -= 1;
    }
    for (int i = 0; i < text_fields_count; i++) {
      ImGui::PushID(i);
      text_fields_vector[i].FieldProperties();
      ImGui::PopID();

      if (!text_fields_vector[i].closable_group) {
        if (i >= 0 && i < text_fields_vector.size()) {
          text_fields_vector.erase(text_fields_vector.begin() + i);
        }
        text_fields_count -= 1;
      }
    }
    if (logo.my_texture == nullptr) {
      logo.my_texture = LoadLogo::ShowLoadLogo();
    }
    else {
      logo.FieldProperties();
    }
    ImGui::SameLine();
    ImGui::TableNextColumn();
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + (210 * 2.5), canvas_p0.y + (297 * 2.5));
    ImRect canvas_rect = ImRect(canvas_p0, canvas_p1);
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    InvisibleButtonEx("canvas2 %d", ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight, ImVec2(60,60), canvas_rect, (void*)logo.my_texture, logo.offset, &logo); 

    for (int i = 0; i < text_fields_count; i++) {
      ImGui::PushID(i);
      InvisibleButtonEx("canvas1 %d", ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight, text_fields_vector[i], canvas_rect);
      ImGui::PopID();
    }
  }
  ImGui::EndTable();
}


bool Images::InvisibleButtonEx(const char* str_id, ImGuiButtonFlags flags, TextField& text_field, ImRect& canvas) {
  float &font_size = text_field.font_size;

  ImGuiContext& g = *GImGui;
  ImGuiWindow* window = ImGui::GetCurrentWindow();
  if (window->SkipItems)
    return false;

  // Cannot use zero-size for InvisibleButton(). Unlike Button() there is not way to fallback using the label size.
  IM_ASSERT(text_field.size.x != 0.0f && text_field.size.y != 0.0f);

  const ImGuiID id = window->GetID(str_id);
  ImVec2 size = ImGui::CalcItemSize(text_field.size, 0.0f, 0.0f);
  if (size.x > canvas.Max.x - canvas.Min.x - text_field.offset.x) {
    size.x = canvas.Max.x - canvas.Min.x - text_field.offset.x - 1;
  }
  if (size.y > canvas.Max.y - canvas.Min.y - text_field.offset.y) {
    size.y = canvas.Max.y - canvas.Min.y - text_field.offset.y - 1;
  }
  const ImRect bb(canvas.Min +text_field.offset, canvas.Min  + text_field.offset + size);

  ImGui::ItemSize(size);
  if (!ImGui::ItemAdd(bb, id))
    return false;

  bool hovered, held;
  bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

  IMGUI_TEST_ENGINE_ITEM_INFO(id, str_id, g.LastItemData.StatusFlags);

  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImVec4 clip_rect(bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y); // AddText() takes a ImVec4* here so let's convert.

  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
    text_field.offset.x += ImGui::GetIO().MouseDelta.x;
    text_field.offset.y += ImGui::GetIO().MouseDelta.y;
  }
  if (ImGui::IsItemHovered()) {
    //std::cout << "Hovered " << std::endl;
    float wheel = ImGui::GetIO().MouseWheel;
    if (wheel) {
      font_size += wheel;
    }
  }

  draw_list->AddRect(bb.Min, bb.Max, IM_COL32(128, 128, 255, 255));
  draw_list->AddText(font, font_size, bb.Min, IM_COL32(0, 0, 0, 255), text_field.text, NULL, size.x, &clip_rect);

  if (text_field.offset.x + size.x> canvas.Max.x  - canvas.Min.x)
    text_field.offset.x = (canvas.Max.x - canvas.Min.x - size.x)-1;
  if (text_field.offset.y + size.y > canvas.Max.y - canvas.Min.y)
    text_field.offset.y = (canvas.Max.y - canvas.Min.y - size.y) - 1;

  if (text_field.offset.x  < 0)
    text_field.offset.x = 1;
  if (text_field.offset.y < 0)
    text_field.offset.y = 1;
  return pressed;
}

bool Images::InvisibleButtonEx(const char* str_id, ImGuiButtonFlags flags, const ImVec2& size_arg, ImRect& canvas, ImTextureID texture_id, ImVec2& offset, Logo* logo) {
 
  static ImVec2 bb_min = canvas.Min;
  
  ImGuiContext& g = *GImGui;
  ImGuiWindow* window = ImGui::GetCurrentWindow();
  if (window->SkipItems)
    return false;

  // Cannot use zero-size for InvisibleButton(). Unlike Button() there is not way to fallback using the label size.
  IM_ASSERT(logo->size.x != 0.0f && logo->size.y != 0.0f);

  const ImGuiID id = window->GetID(str_id);
  ImVec2 size = ImGui::CalcItemSize(logo->size, 0.0f, 0.0f);

  const ImRect bb(canvas.Min + logo->offset, canvas.Min + logo->offset + size);

  ImGui::ItemSize(size);
  if (!ImGui::ItemAdd(bb, id))
    return false;

  bool hovered, held;
  bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

  IMGUI_TEST_ENGINE_ITEM_INFO(id, str_id, g.LastItemData.StatusFlags);

  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImVec4 clip_rect(bb.Min.x, bb.Min.y, bb.Max.x, bb.Max.y); // AddText() takes a ImVec4* here so let's convert.

  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
    logo->offset.x += ImGui::GetIO().MouseDelta.x;
    logo->offset.y += ImGui::GetIO().MouseDelta.y;

  }

  draw_list->AddRect(bb.Min, bb.Min + logo->size, IM_COL32(128, 128, 255, 255));
  draw_list->AddImage((void*)texture_id, bb.Min, bb.Min+ logo->size);
  if (logo->offset.x + size.x > canvas.Max.x - canvas.Min.x)
    logo->offset.x = (canvas.Max.x - canvas.Min.x - size.x) - 1;
  if (logo->offset.y + size.y > canvas.Max.y - canvas.Min.y)
    logo->offset.y = (canvas.Max.y - canvas.Min.y - size.y) - 1;

  if (logo->offset.x < 0)
    logo->offset.x = 1;
  if (logo->offset.y < 0)
    logo->offset.y = 1;
  return pressed;
}

void Images::SetFonts() {
  ImGuiIO& io = ImGui::GetIO();
  ImFontConfig font_config;
  font_config.OversampleH = 2; // FreeType does not support those, reset so stb_truetype will produce similar results
  font_config.OversampleV = 2;
  font_config.RasterizerDensity = 8.0f;
  font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\arial.ttf", 12.0f, &font_config);
  font->Scale = 1.0f;
  
}

