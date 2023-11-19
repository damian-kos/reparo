// In the source file for EditRepair
#include "edit_repair.h"

int EditRepair::instance_count = 0;
bool* EditRepair::show_repair = new bool(false);

EditRepair::EditRepair() { instance_count++;  std::cout << "EditRepair created " << instance_count << std::endl;  }
EditRepair::EditRepair(Repair& repair) : InsertRepair(repair) {
    instance_count++;
    std::cout << "EditRepair created " << instance_count << std::endl;
}

EditRepair::~EditRepair() { instance_count--;  std::cout << "EditRepair destroyed " << instance_count << std::endl;  }

void EditRepair::StateSection() {
    imgui_decorator.SetTestValue(true);
    imgui_decorator.DecorateSeparatorText("STATE: ");
}

