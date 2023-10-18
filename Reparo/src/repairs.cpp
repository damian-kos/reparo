#include "repairs_states.h"



void RepairMenu::RepairMainWin() {
    int selected = -1;
    ImGui::Begin("Repairs");
    repairs.AllToDoRepairs();
    helper.RepairStatesTable(repairs.to_do_repairs, selected);
    ImGui::End();

}
