  #pragma once
// To implement maths operators for ImVec2 (disabled by default to not conflict with using IM_VEC2_CLASS_EXTRA with your own math types+operators), use:

#define IMGUI_DEFINE_MATH_OPERATORS


#include <imgui.h>
#include <imgui_internal.h>
#include "structs.h"
#include "imgui_viewer.h"
#include "database.h"
#include "modals.h"


namespace ImGui {
  //Database db;
  //ModalController modals;

  //struct Customer {
  //public:
  //    Customer()
  //        : phone(""), name(""), surname(""), email("") {}

  //    Customer(const std::string& phone, const std::string& name,
  //        const std::string& surname, const std::string& email)
  //        : phone(phone), name(name), surname(surname), email(email) {}

  //    ~Customer() { /*std::cout << "Customer destroyed" << std::endl;*/ }

  //    std::string phone;
  //    std::string name;
  //    std::string surname;
  //    std::string email;
  //};

  ImFont* font;



  void SetFonts() {
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig font_config;
    font_config.OversampleH = 1; // FreeType does not support those, reset so stb_truetype will produce similar results
    font_config.OversampleV = 1;
    font_config.RasterizerDensity = 2.0f;
    font = io.Fonts->AddFontFromFileTTF("vendor/imgui/misc/fonts/NotoSans-Regular.ttf", 24.0f, &font_config);
    font->Scale = 1.0f;
  }

  void ViewCustomer(const Customer& customer, std::shared_ptr<Customer> t_customer) {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::TableNextColumn();
    ImGui::BeginGroup();
    ImGui::Text("Phone:    "); ImGui::SameLine(); ImGui::Text(customer.phone.c_str());
    ImGui::Text("Name:     "); ImGui::SameLine(); ImGui::Text(customer.name.c_str());
    ImGui::Text("Surname:  "); ImGui::SameLine(); ImGui::Text(customer.surname.c_str());
    ImGui::Text("Email:    "); ImGui::SameLine(); ImGui::Text(customer.email.c_str());
    ImGui::EndGroup();
    if (t_customer) {
      ImGui::TableNextColumn();
      ImGui::SameLine();
      ImGui::BeginGroup();
      std::string tab = "    >>>>    ";
      ImGui::Text((customer.phone != t_customer->phone) ? (tab + (t_customer->phone)).c_str() : "");
      ImGui::Text((customer.name != t_customer->name) ? (tab + (t_customer->name)).c_str() : "");
      ImGui::Text((customer.surname != t_customer->surname ) ? (tab + (t_customer->surname)).c_str() : "");
      ImGui::Text((customer.email != t_customer->email) ? (tab + (t_customer->email)).c_str() : "");
      ImGui::EndGroup();
    }
  }

  void ViewRepair(const Repair& repair, std::shared_ptr<Repair> t_repair, 
                  std::shared_ptr<Customer> t_customer) {
    
    ImGui::BeginTable("table1", 2);
    ImGui::ViewCustomer(repair.customer, t_customer);
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::BeginGroup();
    ImGui::Text("Model:    "); ImGui::SameLine(); ImGui::Text(repair.device.name.c_str());
    ImGui::Text("Category: "); ImGui::SameLine(); ImGui::Text(repair.category.c_str());
    ImGui::Text("Color:    "); ImGui::SameLine(); ImGui::Text(repair.device.color.c_str());
    ImGui::Text("State:    "); ImGui::SameLine(); ImGui::Text(repair.state.c_str());
    ImGui::Text("Price:    "); ImGui::SameLine(); ImGui::Text("%.2f", repair.price);
    ImGui::EndGroup();
    if (t_repair) {
    ImGui::TableNextColumn();
    ImGui::SameLine();
    ImGui::BeginGroup();
    std::string tab = "    >>>>    ";
    ImGui::Text((repair.device.name != t_repair->device.name) ?
                (tab + (t_repair->device.name)).c_str() : "");
    ImGui::Text((repair.category != t_repair->category) ?
                (tab + (t_repair->category)).c_str() : "");
    ImGui::Text((repair.device.color != t_repair->device.color) ?
                (tab + (t_repair->device.color)).c_str() : "");
    ImGui::Text((repair.state != t_repair->state) ?
      tab.append(t_repair->state).c_str() : "");
    double tolerance = 1e-3;
    if (!(std::abs(repair.price - t_repair->price) < tolerance)) {
      ImGui::Text("    >>>>    %.2f", t_repair->price);
    }
    ImGui::EndGroup();
    }
    ImGui::EndTable();
  }

  void SeparatorDecorator(const char* label, bool decorated) {
    if (decorated) {
      ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    }
    else {
      ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    ImGui::PushItemWidth(100);
    ImGui::SeparatorText(label);

    ImGui::PopStyleColor(1);
    ImGui::PopItemWidth();

  }


  void SeparatorTextAlignC(const char* label, int width) { // Dont use 
    ImGuiStyle* style = &ImGui::GetStyle();
    PushID(label);
    ImVec2 label_size = CalcTextSize(label);
    bool ret = ButtonEx(label, label_size + style->FramePadding * 2);
    RenderTextClipped(label_size + style->FramePadding,  style->FramePadding, label, NULL, &label_size, style->ButtonTextAlign);

    // render your text here
    PopID();
  }

  void SeparatorTextAlignR(const char* label) {
    ImGuiStyle* style = &ImGui::GetStyle();
    style->SeparatorTextAlign = ImVec2(1.0f, 0.5f);
    ImGui::SeparatorText(label);
    style->SeparatorTextAlign = ImVec2(0.0f, 0.5f);
  }
  
  void breakApartLF(std::string line, std::vector<std::string>* v)
  {
    int pos;
    std::string delimiter = "\n";

    pos = static_cast<int>(line.find(delimiter));
    while (pos != -1) {
      v->push_back(line.substr(0, pos));
      line.erase(0, pos + 1);

      pos = static_cast<int>(line.find(delimiter));
    }
    v->push_back(line);
  }

  bool CenteredButton(const char* label, const ImVec2& size_arg, std::string test) {
    ImGuiWindow* window = GetCurrentWindow();
    
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(test.c_str(), NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);

    std::vector<std::string> str_vector;
    breakApartLF(test, &str_vector);

    PushID(label);
    bool ret = ButtonEx("##label", size);
    PopID();
    ImVec2 line_size;
    ImVec2 text_start;
    int offset_x, offset_y; 
    ImDrawList* DrawList = ImGui::GetWindowDrawList();
    ImVec2 padding_regular = g.Style.TouchExtraPadding;
    ImVec2 padding_for_resize = g.IO.ConfigWindowsResizeFromEdges ? g.WindowsHoverPadding : padding_regular;

    float font_height = g.FontSize;

    for (int i = 0; i < str_vector.size(); i++)
    {
      line_size = CalcTextSize(str_vector[i].c_str(), NULL, true);
      offset_x = static_cast<int>((size.x - line_size.x) / 2);
      if (line_size.y != 0) {
        offset_y = static_cast<int>((size.y / str_vector.size()) * i + padding_for_resize.y);
      }
      else {
        offset_y = static_cast<int>(font_height * i + padding_for_resize.y);
      }
      text_start.x = bb.Min.x + offset_x;
      text_start.y = bb.Min.y + offset_y;
      DrawList->AddText(text_start, ImGui::GetColorU32(ImGuiCol_Text), str_vector[i].c_str());
      
    }
    return ret;
  }

  void CenteredText(const char* label, const ImVec2& size_arg)
  {
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImVec2 pos2 = ImVec2((pos.x + size.x), (pos.y + size.y));
    const ImRect bb(pos, pos2);

    ItemSize(size, style.FramePadding.y);

    const ImVec2 pos_min = ImVec2((bb.Min.x + style.FramePadding.x), (bb.Min.y + style.FramePadding.y));
    const ImVec2 pos_max = ImVec2((bb.Max.x - style.FramePadding.x), (bb.Max.y - style.FramePadding.y));

    RenderTextClipped(pos_min, pos_max, label, NULL, &label_size, style.ButtonTextAlign, &bb);
  }

  void InputTextWithHintExt(const char* label, const char* hint, HintInputField & field, std::function<bool()> validation_function, bool* feedback) {
    ImGui::InputTextWithHint(label, hint, field.buffer, 128, field.imgui_flags);

    if (ImGui::IsItemEdited() && validation_function) {
      std::cout << "Item is edited " << std::endl;
      field.valid = validation_function();
    }

    if (ImGui::IsItemDeactivated() && feedback) {
      *feedback = true;
    }
  }

  void GetFontV(ImFont* font_t) {
    font = font_t;
  }

  bool ButtonScaled(const char* label, float font_size, ImVec2 button_size)  {
    float old_size = ImGui::GetFont()->Scale;
    ImGui::PushFont(font);
    bool value = false;
    if (ImGui::Button(label, button_size))
      value = true;
    ImGui::PopFont();
    return value;

  }

  void InputTextWithPopup(const char* label, const char* hint, HintInputFieldsW_Popup& field, std::function<bool()> validation_function, bool* selection, HintInputFieldsW_Popup* rel_field, bool* feedback) {
    if (field.input.is_on) {
      ImGui::InputTextWithHintExt(label, hint, field.input, validation_function, feedback);
      if (rel_field) {
        if (rel_field->input.valid && !field.attribute.retreived) {
          int id = Database::GetIDForValueS(label, rel_field->input.buffer);
          Database::ManageSearchState(label, field.attribute, id, field.input.buffer);
          field.attribute.retreived = true;
        }
        if (!rel_field->input.valid && field.attribute.retreived) {
          field.attribute.retreived = false;
        }
      }
      else if (ImGui::IsItemEdited() || ImGui::IsItemActivated()) {
        Database::ManageSearchState(label, field.attribute, field.input.buffer);
      }
      ModalController::PopupOnInputField(field, selection, label);
    }
  }

  bool InvisibleButtonEx(const char* str_id, ImGuiButtonFlags flags, TextField& text_field, const ImRect& canvas) {
    float& font_size = text_field.font_size;

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
    const ImRect bb(canvas.Min + text_field.offset, canvas.Min + text_field.offset + size);

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
      ImU32 col = IM_COL32(128, 128, 255, 255);
      draw_list->AddRect(bb.Min, bb.Max, col);

      ImGui::SetItemUsingMouseWheel();
      draw_list->AddText(NULL, 14, bb.Min - ImVec2(0, 13), IM_COL32(128, 128, 128, 255), text_field.label.c_str(), NULL);
      //std::cout << "Hovered " << std::endl;
      float wheel = ImGui::GetIO().MouseWheel;
      if (wheel) {
        font_size += wheel;
      }
    }
    else {
      ImU32 col = IM_COL32(128, 128, 128, 128);
      draw_list->AddRect(bb.Min, bb.Max, col);
    }

    draw_list->AddText(font, font_size, bb.Min, IM_COL32(0, 0, 0, 255), text_field.text, NULL, size.x, &clip_rect);

    if (text_field.offset.x + size.x > canvas.Max.x - canvas.Min.x)
      text_field.offset.x = (canvas.Max.x - canvas.Min.x - size.x) - 1;
    if (text_field.offset.y + size.y > canvas.Max.y - canvas.Min.y)
      text_field.offset.y = (canvas.Max.y - canvas.Min.y - size.y) - 1;

    if (text_field.offset.x < 0)
      text_field.offset.x = 1;
    if (text_field.offset.y < 0)
      text_field.offset.y = 1;
    return pressed;
  }

  bool InvisibleButtonEx(const char* str_id, ImGuiButtonFlags flags, const ImRect& canvas, ImVec2& offset, Logo* logo) {
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
    draw_list->AddImage((void*)logo->texture, bb.Min, bb.Min + logo->size);
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

  void HelpMarker(const char* desc) {
    {
      ImGui::TextDisabled("(?)");
      if (ImGui::BeginItemTooltip())
      {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
      }
    }
  }
}




