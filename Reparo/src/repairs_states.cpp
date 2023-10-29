#include "repairs_states.h"

void Repairs::RetreiveToDoRepairs() {
    if (!to_do.repairs_of_state_retreived) {
        to_do.repair_of_state = sql.GetRetreiveToDoRepairs(1);
        to_do.repairs_of_state_retreived = true;
    }
}

void Repairs::RetreiveProcessingRepairs() {
    if (!processing.repairs_of_state_retreived) {
        processing.repair_of_state = sql.GetRetreiveToDoRepairs(2);
        processing.repairs_of_state_retreived = true;
    }
}

void Repairs::RetreiveWarrantyRepairs() {
    if (!warranty.repairs_of_state_retreived) {
        warranty.repair_of_state = sql.GetRetreiveToDoRepairs(3);
        warranty.repairs_of_state_retreived = true;
    }
}

void Repairs::RetreiveAwaitingRepairs() {
    if (!awaiting.repairs_of_state_retreived) {
        awaiting.repair_of_state = sql.GetRetreiveToDoRepairs(4);
        awaiting.repairs_of_state_retreived = true;
    }
}

void Repairs::RepairsSelector(int selection) {
    switch (selection) {
        case 0:
            RetreiveToDoRepairs();
            helper.RepairStatesTable(to_do.repair_of_state, to_do.selected);
            break;
        case 1:
            RetreiveProcessingRepairs();
            helper.RepairStatesTable(processing.repair_of_state, processing.selected); 
            break;
        case 2:
            RetreiveWarrantyRepairs();
            helper.RepairStatesTable(warranty.repair_of_state, warranty.selected);
            break;
        case 3:
            RetreiveAwaitingRepairs();
            helper.RepairStatesTable(awaiting.repair_of_state, awaiting.selected);
            break;
        default:
            std::cout << "error" << std::endl;
            break;
    }
}