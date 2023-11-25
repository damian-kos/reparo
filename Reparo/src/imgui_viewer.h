#ifndef IMGUIVIEWER_H_
#define IMGUIVIEWER_H_

#ifndef IMGUI_API
#include <imgui.h>
#endif //IMGUI_API

// USAGE
/*
#include "imguidatechooser.h"
//#include <time.h>   // mandatory when implementing ImGui::TestDateChooser() yourself in your code

// inside a ImGui::Window:
ImGui::TestDateChooser();
*/

struct Customer;  // defined in "structs.h"

namespace ImGui {
    IMGUI_API void ViewCustomer(const Customer& customer);


} // namespace ImGui



#endif
