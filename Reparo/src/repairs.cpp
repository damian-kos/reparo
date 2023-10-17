#include "repairs_states.h"

void RepairMenu::RepairMainWin() {
    ImGui::Begin("Repairs");
    if (ImGui::Button("Test To Do")) {
        Repairs repair;
        repair.AllToDoRepairs();
    }
    ImGui::End();

}
