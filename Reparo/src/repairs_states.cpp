#include "repairs_states.h"

void Repairs::AllToDoRepairs() {
    if (!to_do_retreived) {
        to_do_repairs = sql.GetAllToDoRepairs();
        to_do_retreived = true;
    }
}