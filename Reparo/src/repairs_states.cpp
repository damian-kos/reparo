#include "repairs_states.h"

void Repairs::AllToDoRepairs() {
    to_do_repairs = sql.GetAllToDoRepairs();
}