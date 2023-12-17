#pragma once


enum class ConfirmResult {
    CONIFRM_IDLE = -1,
    CONFIRM_CANCEL,
    CONIFRM_SUBMIT,
};

enum class RepairState {
    EMPTY = 0,
    TO_DO = 1,
    PROCESSING = 2,
    WARRANTY = 3,
    WAITING_FOR_PARTS = 4,
};


//typedef int ConfirmResult;
