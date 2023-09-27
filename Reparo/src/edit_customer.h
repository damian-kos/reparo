#pragma once
// edit_customer.h

#ifndef EDIT_CUSTOMER_H
#define EDIT_CUSTOMER_H

#include "customer_data.h"
#include "customer_input_window.h"

class CustomerEditWindow {

public:
    void Render();
    void SetShouldRender(bool);
    bool GetShouldRender();

};

#endif // EDIT_CUSTOMER_H
