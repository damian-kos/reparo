#include "repairs_states.h"

int selected = -1;


void RepairMenu::RepairMainWin() {
    ImGui::Begin("Repairs");
    repairs.AllToDoRepairs();
    helper.RepairStatesTable(repairs.to_do_repairs, selected);
    ImGui::End();

}
