#include "repairs_states.h"
#include "sql_queries.h"

template <RepairState state>
std::vector<Repair>& Repairs::GetRepairs() {
    static std::vector<Repair> repairs;
    
    if (!repairs.empty()) {
        return curr_repairs;
    }
    if (current_selection == previous_selection) {
        return curr_repairs;
    }
    SQLQuery sql; // Assuming SQLQuery is a class for database access
    curr_repairs = sql.GetRetreiveToDoRepairs(state);
    return repairs;
}

void Repairs::RepairsSelector(int selection, bool& update_repair) {
    current_selection = selection;

        switch (selection) {
            case 0:
                helper.RepairStatesTable(GetRepairs<RepairState_::RepairState_ToDo>(), sel, update_repair);
                break;
            case 1:
                helper.RepairStatesTable(GetRepairs<RepairState_::RepairState_Processing>(), sel, update_repair);
                break;
            case 2:
                helper.RepairStatesTable(GetRepairs<RepairState_::RepairState_Warranty>(), sel, update_repair);
                break;
            case 3:
                helper.RepairStatesTable(GetRepairs<RepairState_::RepairState_AwaitingParts>(), sel, update_repair);
                break;
            default:
                std::cout << "error" << std::endl;
                break;
        }
        if (sel > 0)
            device = curr_repairs[sel];
        
    previous_selection = current_selection;
}

